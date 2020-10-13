﻿using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using DefaultNamespace;
using Nuke.Common;
using Nuke.Common.CI.TeamCity;
using Nuke.Common.IO;
using Nuke.Common.Tooling;
using Nuke.Common.Utilities.Collections;
using static Nuke.Common.IO.FileSystemTasks;

partial class Build
{
    AbsolutePath ResourcesPath => RootDirectory / "resources";
    [Parameter] readonly string AimpPath = @"z:\AIMP\AIMP4.60.2180\";
    [Parameter] readonly int TestTimeout = 5; // 5 minutes

    AbsolutePath IntegrationTestBinPath => SourceDirectory / "IntegrationTest";
    AbsolutePath IntegrationTestPluginPath => (AbsolutePath) Path.Combine(AimpPath, "plugins", "AimpTestRunner");

    Target PrepareTestConfiguration => _ => _
        .Executes(() =>
        {
            var setting = new DefaultSettings();
            setting.Default = new GhprSettings
            {
                OutputPath = TestOutput,
                DataServiceFile = "Ghpr.LocalFileSystem.dll",
                ReportName = "AIMP SDK Test Result",
                ProjectName = "AIMP DotNet SDK",
                LoggerFile = "",
                Sprint = GitRepository.Branch,
                RunName = "",
                RunGuid = "",
                RealTimeGeneration = "True",
                RunsToDisplay = "5",
                TestsToDisplay = "5",
                EscapeTestOutput = true,
                Retention = new Retention
                {
                    Amount = 1000
                }
            };

            setting.Projects = new List<Project>();

            var settingContent = SerializationTasks.JsonSerialize(setting);
            File.WriteAllText(IntegrationTestPluginPath / "Ghpr.NUnit.Settings.json", settingContent);
        });

    Target PrepareIntegrationTests => _ => _
        .Requires(() => AimpPath)
        .Executes(() =>
        {
            DeleteDirectory(IntegrationTestPluginPath);
            EnsureCleanDirectory(IntegrationTestPluginPath);

            Directory.CreateDirectory(IntegrationTestPluginPath);
            EnsureExistingDirectory(IntegrationTestPluginPath);

            IntegrationTestBinPath.GlobDirectories($"**/bin/{Configuration}").ForEach(d =>
            {
                Logger.Info($"Copy plugin files from '{d}' to '{IntegrationTestPluginPath}'");
                var files = Directory.GetFiles(d, "*.dll");
                foreach (var file in files)
                {
                    if (file.EndsWith("aimp_dotnet.dll"))
                    {
                        CopyFile(file, IntegrationTestPluginPath / "AimpTestRunner.dll");
                    }
                    else if (file.EndsWith("AimpTestRunner.dll"))
                    {
                        CopyFile(file, IntegrationTestPluginPath / "AimpTestRunner_plugin.dll");
                    }
                    else
                    {
                        CopyFileToDirectory(file, IntegrationTestPluginPath);
                    }
                }

                CopyFileToDirectory(d / "nunit.engine.addins", IntegrationTestPluginPath);
            });

            CopyDirectoryRecursively(ResourcesPath / "integrationTests", IntegrationTestPluginPath / "resources");
        });

    Target ExecuteIntegrationTests => _ => _
        .Requires(() => AimpPath)
        .DependsOn(PrepareIntegrationTests, PrepareTestConfiguration)
        .Executes(() =>
        {
            var testResultFile = IntegrationTestPluginPath / "integration.tests.xml";
            var testResultLogFile = IntegrationTestPluginPath / "integration.tests.log";

            if (File.Exists(testResultFile))
                File.Delete(testResultFile);

            if (File.Exists(testResultLogFile))
                File.Delete(testResultLogFile);

            var testsTimeoutWorker = new BackgroundWorker();

            var p = ProcessTasks.StartProcess(Path.Combine(AimpPath, "AIMP.exe"), "/DEBUG", AimpPath, timeout: TestTimeout * 60000);
            var res = p.WaitForExit();

            if (res)
            {
                if (!File.Exists(testResultFile))
                {
                    LogError("Unable to run integration tests.");
                }
                else
                {
                    CopyFileToDirectory(testResultFile, OutputDirectory);
                    CopyFileToDirectory(testResultLogFile, OutputDirectory);

                    TeamCity.Instance?.WriteMessage(File.ReadAllText(testResultLogFile));
                }
            }
            else
            {
                LogError("Process was ended by timeout. Try to increase `TestTimeout` parameter. Check logs. Check that plugin was activated at AIMP settings.");
            }
        });

    private void LogError(string message)
    {
        if (TeamCity.Instance != null)
        {
            TeamCity.Instance.WriteError(message);
        }
        else
        {
            Logger.Error(message);
        }
    }
}
