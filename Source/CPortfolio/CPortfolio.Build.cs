using UnrealBuildTool;

public class CPortfolio : ModuleRules
{
    public CPortfolio(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("InputCore");

        PublicDependencyModuleNames.Add("Niagara");

        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GamePlayTasks");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("LevelSequence");
    }
}