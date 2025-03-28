if (Test-Path "$PSScriptRoot/build") {
    Remove-Item -Recurse -Force "$PSScriptRoot/build"
}

New-Item -ItemType Directory -Path "$PSScriptRoot/build" | Out-Null

cmake -G "MinGW Makefiles" -S "$PSScriptRoot" -B "$PSScriptRoot/build"

cmake --build "$PSScriptRoot/build"

Copy-Item "$PSScriptRoot/build/InputListener.dll" "$PSScriptRoot/InputListener.dll"

Copy-Item "$PSScriptRoot/build/InputListener.dll" "$PSScriptRoot/InputListenerC/InputListener.dll"
