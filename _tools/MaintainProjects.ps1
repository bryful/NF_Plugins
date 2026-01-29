# --- 実行スイッチ (TRUE で実行、FALSE でスキップ) ---
$doUpdatePlatformToolset   = $false
$doUpdateOutDirDebug       = $true
$doUpdateOutDirRelease     = $true
$doUpdateWarningLevel       = $true
$doDisableWarnings          = $true
$doUpdateDebuggerCommand    = $true  # 追加: デバッグコマンドの更新

# --- 設定値 ---
$targetFolder = "./" 
$newOutDirDebug = "..\..\NF-Plugins_debug\"
$newOutDirRelease = "..\..\NF-Plugins_release\"
$newPlatformToolset = "v145"
$newWarningLevel = "Level4"
$disableWarnings = "4201;4103;4189;4505;4996;4100;4458;4456"
# デバッグコマンドのパス
$newDebugCommand = "C:\Program Files\Adobe\Adobe After Effects 2024\Support Files\AfterFX.exe"

# 全プロジェクト取得
$projFiles = Get-ChildItem -Path $targetFolder -Filter "*.vcxproj" -Recurse

foreach ($file in $projFiles) {
    Write-Host "Checking: $($file.Name)" -ForegroundColor Cyan
    [xml]$xml = Get-Content $file.FullName
    $ns = New-Object Xml.XmlNamespaceManager $xml.NameTable
    $ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

    $isChanged = $false

    # 1. PlatformToolset の同期
    if ($doUpdatePlatformToolset) {
        $configGroups = $xml.SelectNodes("//ns:PropertyGroup[@Label='Configuration']", $ns)
        foreach ($group in $configGroups) {
            if ($group.PlatformToolset -ne $newPlatformToolset) {
                $group.PlatformToolset = $newPlatformToolset
                $isChanged = $true
            }
        }
    }

    # 2. OutDir の同期
    if ($doUpdateOutDirDebug -or $doUpdateOutDirRelease) {
        $outDirNodes = $xml.SelectNodes("//ns:PropertyGroup/ns:OutDir", $ns)
        foreach ($node in $outDirNodes) {
            if ($doUpdateOutDirDebug -and $node.Condition -like "*Debug*" -and $node.InnerText -ne $newOutDirDebug) {
                $node.InnerText = $newOutDirDebug
                $isChanged = $true
            }
            elseif ($doUpdateOutDirRelease -and $node.Condition -like "*Release*" -and $node.InnerText -ne $newOutDirRelease) {
                $node.InnerText = $newOutDirRelease
                $isChanged = $true
            }
        }
    }

    # 3. デバッグコマンドの設定 (Debug構成のみ)
    if ($doUpdateDebuggerCommand) {
        # Debug構成の PropertyGroup を特定 (Labelなしの汎用グループを探す)
        $debugPropGroup = $xml.SelectSingleNode("//ns:PropertyGroup[contains(@Condition, 'Debug') and not(@Label)]", $ns)
        if ($null -ne $debugPropGroup) {
            if ($null -eq $debugPropGroup.LocalDebuggerCommand) {
                $newElem = $xml.CreateElement("LocalDebuggerCommand", $ns.LookupNamespace("ns"))
                $newElem.InnerText = $newDebugCommand
                $debugPropGroup.AppendChild($newElem)
                $isChanged = $true
            }
            elseif ($debugPropGroup.LocalDebuggerCommand -ne $newDebugCommand) {
                $debugPropGroup.LocalDebuggerCommand = $newDebugCommand
                $isChanged = $true
            }
        }
    }

    # 4. ClCompile 設定 (WarningLevel & DisableSpecificWarnings)
    if ($doUpdateWarningLevel -or $doDisableWarnings) {
        $clCompileNodes = $xml.SelectNodes("//ns:ItemDefinitionGroup[contains(@Condition, 'Debug') or contains(@Condition, 'Release')]/ns:ClCompile", $ns)
        foreach ($node in $clCompileNodes) {
            if ($doUpdateWarningLevel -and $node.WarningLevel -ne $newWarningLevel) {
                $node.WarningLevel = $newWarningLevel
                $isChanged = $true
            }
            if ($doDisableWarnings) {
                $val = $disableWarnings + ";%(DisableSpecificWarnings)"
                if ($null -eq $node.DisableSpecificWarnings) {
                    $newElem = $xml.CreateElement("DisableSpecificWarnings", $ns.LookupNamespace("ns"))
                    $newElem.InnerText = $val
                    $node.AppendChild($newElem)
                    $isChanged = $true
                }
                elseif ($node.DisableSpecificWarnings -ne $val) {
                    $node.DisableSpecificWarnings = $val
                    $isChanged = $true
                }
            }
        }
    }

    # 変更があった場合のみ保存
    if ($isChanged) {
        $xml.Save($file.FullName)
        Write-Host "  -> Updated." -ForegroundColor Green
    } else {
        Write-Host "  -> No changes needed." -ForegroundColor DarkGray
    }
}