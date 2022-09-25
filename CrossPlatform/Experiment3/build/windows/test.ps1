$Path = "C:\Data\Indie\Development\sandbox-2\CrossPlatform\Experiment3\build\windows\..\..\third-party\test.zip"
$Location = Get-Location
$Path2 = "$Location\..\..\third-party\test.zip"
Write-Output $Path
Write-Output $Path2
$WebClient = New-Object System.Net.WebClient
$WebClient.DownloadFile("https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip", "$Path2")