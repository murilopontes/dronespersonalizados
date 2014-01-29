param($installPath, $toolsPath, $package, $project)

$mogreDependencies =  "
if `$(ConfigurationName) == Debug (
    if EXIST `"`$(SolutionDir)\packages\Mogre1.7.4.1.0.6\tools\Mogre.Dependencies.Debug\Mogre.dll`" (
        copy `"`$(SolutionDir)\packages\Mogre1.7.4.1.0.6\tools\Mogre.Dependencies.Debug\*`" `"`$(TargetDir))`"
    ) ELSE (
        copy `"`$(SolutionDir)\packages\Mogre1.7.4.1.0.6\tools\Mogre.Dependencies.Release\*`" `"`$(TargetDir)`"
    )
) ELSE (
    copy `"`$(SolutionDir)\packages\Mogre1.7.4.1.0.6\tools\Mogre.Dependencies.Release\*`" `"`$(TargetDir)`"
)"

# Get the current Post Build Event cmd
$currentPostBuildCmd = $project.Properties.Item("PostBuildEvent").Value

# Append our post build command if it's not already there
if (!$currentPostBuildCmd.Contains($mogreDependencies)) {
    $project.Properties.Item("PostBuildEvent").Value += $mogreDependencies
}