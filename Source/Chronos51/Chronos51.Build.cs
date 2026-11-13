using UnrealBuildTool;

public class Chronos51 : ModuleRules
{
    public Chronos51(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "Niagara",
            "UMG",
            "Slate",
            "SlateCore",
            "Json",
            "JsonUtilities",
            "HTTP"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Projects",
            "RenderCore",
            "RHI",
            "NavigationSystem",
            "AIModule",
            "PhysicsCore"
        });

        // Enable C++20 features
        CppStandard = CppStandardVersion.Cpp20;
        
        // Optimize for development iteration
        if (Target.Configuration == UnrealTargetConfiguration.Development)
        {
            OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        }

        // Enable modules for GAS
        PublicIncludePaths.AddRange(new string[] {
            ModuleDirectory + "/Public",
            ModuleDirectory + "/Private"
        });
    }
}