param([string]$themesDir)

$qrcPath = Join-Path $themesDir "themes.qrc"

# Get all generated CSS files.
$cssFiles = Get-ChildItem $themesDir -Filter "generated--*.css"

# Load or create QRC XML.
if (Test-Path $qrcPath)
{
    [xml]$qrc = Get-Content $qrcPath
}
else
{
    $qrc = New-Object System.Xml.XmlDocument

    $rcc = $qrc.CreateElement("RCC")
    $qresource = $qrc.CreateElement("qresource")
    $qresource.SetAttribute("prefix", "/themes")

    $rcc.AppendChild($qresource) | Out-Null
    $qrc.AppendChild($rcc) | Out-Null
}

$qresourceNode = $qrc.RCC.qresource

# Track existing aliases.
$existing = @{ }
foreach ($file in $qresourceNode.file)
{
    if ($file.alias)
    {
        $existing[$file.alias] = $true
    }
}

# Add missing entries.
foreach ($css in $cssFiles)
{

    $name = $css.BaseName

    if ( $name.StartsWith("generated--"))
    {
        # Strip prefix.
        $alias = $name.Substring("generated--".Length)

        # Strip .css extension in alias.
        if (-not $existing.ContainsKey($alias))
        {
            $fileNode = $qrc.CreateElement("file")
            $fileNode.SetAttribute("alias", $alias)

            $fileNode.InnerText = $css.Name
            $qresourceNode.AppendChild($fileNode) | Out-Null

            Write-Host "Added to QRC: $alias"
        }
    }
}

# Save updated QRC.
$qrc.Save($qrcPath)