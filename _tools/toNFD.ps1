# 対象のディレクトリパス
$targetDir = "."

# 深い階層から順に取得
$items = Get-ChildItem -Path $targetDir -Recurse | Sort-Object { $_.FullName.Length } -Descending

foreach ($item in $items) {
    $oldName = $item.Name
    
    # 1. NFC（FormC）に正規化
    $normalizedName = $oldName.Normalize([System.Text.NormalizationForm]::FormC)
    
    # 2. 波ダッシュ(U+301C)を全角チルダ(U+FF5E)に置換
    # [char]0x301C = 〜 , [char]0xFF5E = ～
    $newName = $normalizedName.Replace([char]0x301C, [char]0xFF5E)

    # 変更が必要な場合のみ実行
    if ($oldName -ne $newName) {
        try {
            $type = if ($item.PSIsContainer) { "Directory" } else { "File" }
            Write-Host "Renaming $type: '$oldName' -> '$newName'" -ForegroundColor Cyan
            
            Rename-Item -Path $item.FullName -NewName $newName -ErrorAction Stop
        }
        catch {
            Write-Warning "Failed to rename '$($item.FullName)': $_"
        }
    }
}