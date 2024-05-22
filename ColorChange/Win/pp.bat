cl /I "C:\Users\51000291\Documents\AdobeSDK\AfterEffectsSDK AE2023\Examples\NF_Plugins\Skelton\Win\..\..\..\Headers" /EP ".."\\"SkeltonPiPL.r" > "x64\Debug\"\\"SkeltonPiPL.rr"
"C:\Users\51000291\Documents\AdobeSDK\AfterEffectsSDK AE2023\Examples\NF_Plugins\Skelton\Win\..\..\..\Resources\PiPLTool" "x64\Debug\SkeltonPiPL.rr" "x64\Debug\SkeltonPiPL.rrc"
cl /D "MSWindows" /EP x64\Debug\SkeltonPiPL.rrc >               "C:\Users\51000291\Documents\AdobeSDK\AfterEffectsSDK AE2023\Examples\NF_Plugins\Skelton\Win\"\\"SkeltonPiPL".rc
