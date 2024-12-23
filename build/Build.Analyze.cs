﻿//  ----------------------------------------------------
//  AIMP DotNet SDK
// 
//  Copyright (c) 2014 - 2023 Evgeniy Bogdan
//  https://github.com/martin211/aimp_dotnet
// 
//  Mail: mail4evgeniy@gmail.com
//  ----------------------------------------------------

using System.IO;
using Aimp.DotNet.Build;
using Nuke.Common;
using Nuke.Common.Tooling;
using Nuke.Common.Tools.DotNet;
using Nuke.Common.Tools.MSBuild;
using Nuke.Common.Tools.SonarScanner;
using static Nuke.Common.Tools.MSBuild.MSBuildTasks;

partial class Build
{
    [Parameter] readonly bool SonarVerbose;
    [Parameter] readonly string SonarUrl;
    [Parameter] readonly string SonarUser;
    [Parameter] readonly string SonarPassword;
    [Parameter] readonly string SonarProjectKey;
    [Parameter] readonly string SonarProjectName;

    Target SonarQube => _ => _
        .Requires(() => SonarUrl, () => SonarUser, () => SonarProjectKey, () => SonarProjectName)
        .DependsOn(Restore, Version)
        .Executes(() =>
            {
                var framework = "net5.0";
                var configuration = new SonarBeginSettings()
                    .SetProjectKey(SonarProjectKey)
                    .SetIssueTrackerUrl(SonarUrl)
                    .SetServer(SonarUrl)
                    .SetVersion(_version)
                    .SetLogin(SonarUser)
                    .SetPassword(SonarPassword)
                    .SetName(SonarProjectName)
                    .SetBranchName(GitRepository.Branch)
                    .SetFramework(framework);

                if (SonarVerbose)
                    configuration = configuration
                        .EnableVerbose();

                if (File.Exists(PvsReportPath))
                {
                    configuration = configuration.SetPvsStudioReportPath(PvsReportPath);
                }

                if (GitRepository.Branch != null && !GitRepository.Branch.Contains(ReleaseBranchPrefix))
                {
                    configuration = configuration.SetVersion(_version);
                }

                //configuration = configuration.SetProjectBaseDir(SourceDirectory);

                if (!string.IsNullOrWhiteSpace(RequestSourceBranch) && !string.IsNullOrWhiteSpace(RequestTargetBranch))
                {
                    configuration = configuration
                        .SetPullRequestBase(RequestSourceBranch)
                        .SetPullRequestBranch(RequestTargetBranch)
                        .SetPullRequestKey(RequestId);
                }

                var path = NuGetToolPathResolver.GetPackageExecutable(
                    packageId: "dotnet-sonarscanner",
                    packageExecutable: "SonarScanner.MSBuild.dll",
                    framework: framework);

                configuration = configuration.SetProcessToolPath(path);

                var arguments = $"{path} {configuration.GetProcessArguments().RenderForExecution()}";

                DotNetTasks.DotNet(arguments);
            }, () =>
            {
                MSBuild(c => c
                    .SetConfiguration(GetConfiguration())
                    .SetProcessToolPath(MsBuildPath)
                    .SetTargets("Rebuild")
                    .SetSolutionFile(Solution)
                    .EnableNodeReuse());
            },
            () =>
            {
                var framework = "net5.0";
                var path = NuGetToolPathResolver.GetPackageExecutable(
                    packageId: "dotnet-sonarscanner",
                    packageExecutable: "SonarScanner.MSBuild.dll",
                    framework: framework);

                var configuration = new SonarScannerEndSettings()
                    .SetLogin(SonarUser)
                    .SetPassword(SonarPassword)
                    .SetFramework(framework)
                    //.SetProjectBaseDir(SourceDirectory)
                    .EnableProcessLogOutput();

                var arguments = $"{path} {configuration.GetProcessArguments().RenderForExecution()}";

                DotNetTasks.DotNet(arguments);
            });

    Target PvsStudio => _ => _
        .Executes(() =>
        {
            //PvsStudioTasks.PvsStudioRun(c => c
            //    .SetConfiguration(Configuration)
            //    .SetTarget(Solution)
            //    .SetOutput(PvsReportPath));
        });
}