param(
    [Parameter(Position = 0)]
    [ValidatePattern('^(stable|latest|\d+\.\d+\.\d+(-[^\s]+)?)$')]
    [string]$Target = "latest"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$ProgressPreference = 'SilentlyContinue'

# ========== 调试模式：详细打印每一步 ==========
# 设置为 $true 开启详细日志，$false 关闭
$DEBUG_MODE = $true

if ($DEBUG_MODE) {
    # Trace 1: 打印每一行执行的脚本
    # Trace 2: 额外打印变量赋值、函数调用
    Set-PSDebug -Trace 1

    # 拦截所有外部命令调用，打印实际执行的命令
    $OriginalPref = $DebugPreference
    $DebugPreference = "Continue"
    Write-Debug "=== 调试模式已开启 ==="
    Write-Debug "脚本路径: $PSCommandPath"
    Write-Debug "当前目录: $PWD"
    Write-Debug "目标版本: $Target"
    Write-Debug "平台架构: $env:PROCESSOR_ARCHITECTURE"
    Write-Debug "========================`n"
    $DebugPreference = $OriginalPref
}
# ================================================

# Check for 32-bit Windows
if (-not [Environment]::Is64BitProcess) {
    Write-Error "Claude Code does not support 32-bit Windows. Please use a 64-bit version of Windows."
    exit 1
}

$DOWNLOAD_BASE_URL = "https://downloads.claude.ai/claude-code-releases"
$DOWNLOAD_DIR = "$env:USERPROFILE\.claude\downloads"

# Use native ARM64 binary on ARM64 Windows, x64 otherwise
if ($env:PROCESSOR_ARCHITECTURE -eq "ARM64") {
    $platform = "win32-arm64"
}
else {
    $platform = "win32-x64"
}
New-Item -ItemType Directory -Force -Path $DOWNLOAD_DIR | Out-Null
if ($DEBUG_MODE) { Write-Host "[STEP 1] 创建下载目录: $DOWNLOAD_DIR" -ForegroundColor Cyan }

# Always download latest version (which has the most up-to-date installer)
if ($DEBUG_MODE) { Write-Host "[STEP 2] 获取最新版本号..." -ForegroundColor Cyan }
try {
    if ($DEBUG_MODE) { Write-Host "  -> GET $DOWNLOAD_BASE_URL/latest" -ForegroundColor Gray }
    $version = Invoke-RestMethod -Uri "$DOWNLOAD_BASE_URL/latest" -ErrorAction Stop
    if ($DEBUG_MODE) { Write-Host "  <- 版本: $version" -ForegroundColor Green }
}
catch {
    Write-Error "Failed to get latest version: $_"
    exit 1
}

# Reject non-version content (e.g. an HTML error page) before it reaches the manifest URL
if ($version -notmatch '^\d+\.\d+\.\d+') {
    Write-Error "Failed to get a valid version from downloads.claude.ai (got unexpected content). This can happen if the download service is unreachable or not available in your region - see https://www.anthropic.com/supported-countries"
    exit 1
}

if ($DEBUG_MODE) { Write-Host "[STEP 3] 获取 manifest 和校验和..." -ForegroundColor Cyan }
try {
    if ($DEBUG_MODE) { Write-Host "  -> GET $DOWNLOAD_BASE_URL/$version/manifest.json" -ForegroundColor Gray }
    $manifest = Invoke-RestMethod -Uri "$DOWNLOAD_BASE_URL/$version/manifest.json" -ErrorAction Stop
    $checksum = $manifest.platforms.$platform.checksum

    if (-not $checksum) {
        Write-Error "Platform $platform not found in manifest"
        exit 1
    }
}
catch {
    Write-Error "Failed to get manifest: $_"
    exit 1
}

# Download and verify
$binaryPath = "$DOWNLOAD_DIR\claude-$version-$platform.exe"
if ($DEBUG_MODE) { Write-Host "[STEP 4] 下载二进制文件..." -ForegroundColor Cyan }
try {
    if ($DEBUG_MODE) { Write-Host "  -> GET $DOWNLOAD_BASE_URL/$version/$platform/claude.exe" -ForegroundColor Gray }
    if ($DEBUG_MODE) { Write-Host "  -> 保存到: $binaryPath" -ForegroundColor Gray }
    Invoke-WebRequest -Uri "$DOWNLOAD_BASE_URL/$version/$platform/claude.exe" -OutFile $binaryPath -ErrorAction Stop
    if ($DEBUG_MODE) { Write-Host "  <- 下载完成" -ForegroundColor Green }
}
catch {
    Write-Error "Failed to download binary: $_"
    if (Test-Path $binaryPath) {
        Remove-Item -Force $binaryPath
    }
    exit 1
}

# Calculate checksum
if ($DEBUG_MODE) { Write-Host "[STEP 5] 校验 SHA256..." -ForegroundColor Cyan }
$actualChecksum = (Get-FileHash -Path $binaryPath -Algorithm SHA256).Hash.ToLower()
if ($DEBUG_MODE) { Write-Host "  期望: $checksum" -ForegroundColor Gray }
if ($DEBUG_MODE) { Write-Host "  实际: $actualChecksum" -ForegroundColor Gray }

if ($actualChecksum -ne $checksum) {
    Write-Error "Checksum verification failed"
    Remove-Item -Force $binaryPath
    exit 1
}

# Run claude install to set up launcher and shell integration
Write-Output "Setting up Claude Code..."
if ($DEBUG_MODE) { Write-Host "[STEP 6] 执行安装..." -ForegroundColor Cyan }
try {
    if ($Target) {
        if ($DEBUG_MODE) { Write-Host "  -> & $binaryPath install $Target" -ForegroundColor Gray }
        & $binaryPath install $Target
    }
    else {
        if ($DEBUG_MODE) { Write-Host "  -> & $binaryPath install" -ForegroundColor Gray }
        & $binaryPath install
    }
    # Native exit codes don't trigger $ErrorActionPreference - capture explicitly
    if ($DEBUG_MODE) { Write-Host "  <- 安装退出码: $installExitCode" -ForegroundColor $(if ($installExitCode -eq 0) { "Green" } else { "Red" }) }
}
finally {
    try {
        # Clean up downloaded file
        # Wait a moment for any file handles to be released
        if ($DEBUG_MODE) { Write-Host "[STEP 7] 清理临时文件..." -ForegroundColor Cyan }
        Start-Sleep -Seconds 1
        if ($DEBUG_MODE) { Write-Host "  -> Remove-Item $binaryPath" -ForegroundColor Gray }
        Remove-Item -Force $binaryPath
        if ($DEBUG_MODE) { Write-Host "  <- 清理完成" -ForegroundColor Green }
    }
    catch {
        Write-Warning "Could not remove temporary file: $binaryPath"
    }
}

if ($installExitCode -ne 0) {
    Write-Error "Installation failed (exit code $installExitCode)"
    exit $installExitCode
}

Write-Output ""
Write-Output "$([char]0x2705) Installation complete!"
Write-Output ""
