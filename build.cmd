@echo off
rem ============================================================================
rem Windows entry point for the Rebang build. Used by Visual Studio (Rebang.sln)
rem and usable from any prompt:  build.cmd [-t projectg|wangreal] build|rebuild|clean|verify
rem ============================================================================
setlocal enabledelayedexpansion

cd /d "%~dp0"

set "PYTHONUTF8=1"
set "PYTHON="

:: Procura por versões do Python na máquina
py -3 --version >nul 2>&1 && set "PYTHON=py -3"
if not defined PYTHON (python --version >nul 2>&1 && set "PYTHON=python")
if not defined PYTHON (python3 --version >nul 2>&1 && set "PYTHON=python3")

if not defined PYTHON goto :installpython

:: Valida a versão do Python (3.10+)
%PYTHON% -c "import sys; sys.exit(0 if sys.version_info >= (3, 10) else 1)" >nul 2>&1
if errorlevel 1 goto :installpython

goto :runbuild

:installpython
echo.
echo ============================================================================
echo [Rebang Build] Python 3.10+ nao encontrado ou versao incompativel.
echo Buscando e instalando a versao mais recente do Python automaticamente...
echo ============================================================================

:: Detecta a arquitetura do processador (AMD64, ARM64 ou x86)
set "PY_ARCH=win32"
if /i "%PROCESSOR_ARCHITECTURE%"=="AMD64" set "PY_ARCH=amd64"
if /i "%PROCESSOR_ARCHITEW6432%"=="AMD64" set "PY_ARCH=amd64"
if /i "%PROCESSOR_ARCHITECTURE%"=="ARM64" set "PY_ARCH=arm64"

:: Usa o PowerShell para consultar a API oficial do Python e baixar a versão estável mais recente para a arquitetura correta
set "INSTALLER=%TEMP%\python_installer.exe"

echo Consultando site oficial do Python para obter o link mais recente (%PY_ARCH%)...
powershell -Command "$ErrorActionPreference = 'Stop'; try { $json = Invoke-RestMethod -Uri 'https://www.python.org/api/v2/downloads/release/'; $rel = $json | Where-Object { $_.name -match 'Python 3\.\d+\.\d+' -and $_.is_active -eq $true } | Select-Object -First 1; $file = $rel.files | Where-Object { $_.os == 'windows' -and $_.arch -eq '%PY_ARCH%' -and $_.file_type == 'executable' } | Select-Object -First 1; if ($file) { Invoke-WebRequest -Uri $file.url -OutFile '%INSTALLER%' } else { exit 1 } } catch { exit 1 }"

if not exist "%INSTALLER%" (
    echo [ERRO] Nao foi possivel baixar o instalador automaticamente.
    echo Baixe manualmente em: https://www.python.org/downloads/
    goto :pailexit
)

echo Instalando o Python silenciosamente...
"%INSTALLER%" /quiet InstallAllUsers=0 PrependPath=1 Include_test=0

:: Remove o instalador temporário
del /q "%INSTALLER%" >nul 2>&1

:: Atualiza as variáveis de ambiente da sessão atual buscando o caminho padrão do Python do usuário
for /d %%D in ("%LocalAppData%\Programs\Python\Python3*") do (
    set "PYTHON_PATH=%%D"
)

if defined PYTHON_PATH (
    set "PATH=%PYTHON_PATH%;%PYTHON_PATH%\Scripts\;%PATH%"
    set "PYTHON=python"
)

:: Verifica se a instalação funcionou
%PYTHON% --version >nul 2>&1
if errorlevel 1 (
    echo [ERRO] A instalacao automatica falhou ou o Python nao esta acessivel.
    goto :pailexit
)

echo [Sucesso] Python instalado e configurado com sucesso!

:runbuild
:: Verifica se o script de build existe
if not exist "%~dp0tools\build.py" (
    echo [ERRO] O script auxiliar 'tools\build.py' nao foi encontrado!
    goto :pailexit
)

echo [Rebang Build] Executando build via Python...
%PYTHON% "%~dp0tools\build.py" %*
set "BUILD_EXIT_CODE=%ERRORLEVEL%"

if %BUILD_EXIT_CODE% neq 0 (
    echo.
    echo ============================================================================
    echo [ERRO] O processo de compilacao falhou com o codigo: %BUILD_EXIT_CODE%
    echo ============================================================================
    goto :pailexit
)

exit /b 0

:pailexit
echo.
@echo off
pause
exit /b 1