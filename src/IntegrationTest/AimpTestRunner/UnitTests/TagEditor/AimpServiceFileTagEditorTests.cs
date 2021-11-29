﻿// ----------------------------------------------------
// 
// AIMP DotNet SDK
// 
// Copyright (c) 2014 - 2020 Evgeniy Bogdan
// https://github.com/martin211/aimp_dotnet
// 
// Mail: mail4evgeniy@gmail.com
// 
// ----------------------------------------------------

using AIMP.SDK;
using AIMP.SDK.TagEditor;
using Aimp.TestRunner.TestFramework;
using NUnit.Framework;

namespace Aimp.TestRunner.UnitTests.TagEditor
{
    [TestFixture]
    public class AimpServiceFileTagEditorTests : AimpIntegrationTest
    {
        [Test]
        public void EditFile_ShouldReturnTagEditorForFile()
        {
            ExecuteInMainThread(() =>
            {
                var result = Player.ServiceFileTagEditor.EditFile(TrackPath1);
                this.AreEqual(ActionResultType.OK, result.ResultType, "Unable to get tag editor for file.");
                this.NotNull(result.Result);
                result.Result.Dispose();
            });
        }

        
        [TestCase(TagType.ID3v1)]
        [TestCase(TagType.ID3v2)]
        public void EditTag_FilePath_ShouldBeOK(TagType tag)
        {
            ExecuteInMainThread(() =>
            {
                var result = Player.ServiceFileTagEditor.EditTag(TrackPath1, tag);
                this.AreEqual(ActionResultType.OK, result.ResultType, "Unable to edit tag for file.");
                this.NotNull(result.Result);
                result.Result.Dispose();
            });
        }

        [TestCase(TagType.ApeV2)]
        [TestCase(TagType.M4A)]
        [TestCase(TagType.Vorbis)]
        [TestCase(TagType.WMA)]
        public void EditTag_FilePath_ShouldBeFailForWrongFileFormat(TagType tag)
        {
            ExecuteInMainThread(() =>
            {
                var result = Player.ServiceFileTagEditor.EditTag(TrackPath1, tag);
                this.AreEqual(ActionResultType.Fail, result.ResultType, "Unable to edit tag for file.");
                this.Null(() => result.Result);
            });
        }
    }
}