%SYSTEMROOT%\SYSTEM32\REG.EXE ADD "HKCU\Software\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION" /v NobetterUltimateInternet.exe /t REG_DWORD /d 0x2AF9 /f
%SYSTEMROOT%\SYSTEM32\REG.EXE ADD "HKLM\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION" /v NobetterUltimateInternet.exe /t REG_DWORD /d 0x2AF9 /f
%SYSTEMROOT%\SYSTEM32\REG.EXE ADD "HKLM\SOFTWARE\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION" /v NobetterUltimateInternet.exe /t REG_DWORD /d 0x2AF9 /f
%SYSTEMROOT%\SYSTEM32\REG.EXE ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v CertificateRevocation /t REG_DWORD /d 0x00 /f
%SYSTEMROOT%\SYSTEM32\REG.EXE ADD "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v WarnonBadCertRecving /t REG_DWORD /d 0x00 /f
EXIT