﻿using System;
using System.Collections.Generic;

namespace AIMP.SDK.Services.PlayListManager
{
    [Flags]
    public enum PlayListFlags
    {
        NOCHECKFORMAT,

        NOEXPAND,

        NOASYNC,

        FILEINFO
    }

    public enum PlayListFilePosition
    {
        RandomPosition = -2,

        EndPosition = -1,

        CurrentPosition = 0
    }

    public enum PlayListSort
    {
        TITLE,
        FILENAME,
        DURATION,
        ARTIST,
        INVERSE,
        RANDOMIZE
    }

    [Flags]
    public enum PlayListCloseFlag
    {
        None = 0,

        ForceRemove = 1,

        ForceUnload = 2
    }

    [Flags]
    public enum PlayListGetFilesFlag
    {
        All = 0,

        SelectedOnly = 0x1,

        VisibleOnly = 0x2,

        CollapseVirtual = 0x4
    }

    public enum PlayListSortComapreResult
    {
        TheSame = 0,
        LessThen = -1,
        GrantThen = 1
    }

    public class AimpPlayListChangedArgs : EventArgs
    {
        
    }

    public interface IAimpPlayList
    {
        //event EventHandler Activated;

        //event EventHandler Removed;

        //event EventHandler<AimpPlayListChangedArgs> Changed;

        /// <summary>
        /// Gets the identifier.
        /// </summary>
        /// <value>
        /// The identifier.
        /// </value>
        string Id { get; }

        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        string Name { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether [read only].
        /// </summary>
        bool ReadOnly { get; set; }

        object FocusedObject { get; set; }

        bool GrouppingOvveriden { get; set; }

        bool Groupping { get; set; }

        string GrouppingTemplate { get; set; }

        bool GrouppingAutomerging { get; set; }

        bool FormatingOverride { get; set; }

        string FirstLineTemplate { get; set; }

        string SecondLineTemplate { get; set; }

        bool ViewOverride { get; set; }

        bool ShowDuration { get; set; }

        bool ShowExpandButtons { get; set; }

        bool ShowMarks { get; set; }

        bool ShowNumbers { get; set; }

        bool ShowAbsoluteNumbers { get; set; }

        bool ShowSecondLine { get; set; }

        bool ShowSwitches { get; set; }

        int SelectedIndex { get; set; }

        int PlaybackCursor { get; set; }

        int PlayingIndex { get; set; }

        double Duration { get; }

        double Size { get; }

        string PreImage { get; set; }

        void Add(string fileUrl, PlayListFlags flags, PlayListFilePosition filePosition);

        void Add(IAimpFileInfo fileInfo, PlayListFlags flags, PlayListFilePosition filePosition);

        void AddList(IList<string> fileUrlList, PlayListFlags flags, PlayListFilePosition filePosition);

        void AddList(IList<IAimpFileInfo> fileUrlList, PlayListFlags flags, PlayListFilePosition filePosition);

        void Delete(IAimpPlayListItem item);

        void Delete(int index);

        // TODO: implement Delete3
        //void Delete(bool physically);

        void DeleteAll();

        void Sort(PlayListSort sort);

        void Sort(Func<IAimpPlayListItem, IAimpPlayListItem, String, PlayListSortComapreResult> compareFunc, String userData);

        //TODO: implement Sort2, Sort3
        //void Sort();

        void BeginUpdate();

        void EndUpdate();

        void Close(PlayListCloseFlag closeFlag);

        IList<string> GetFiles(PlayListGetFilesFlag filesFlag);

        // TODO: Add MergeGroup

        void ReloadFromPreimage();

        void ReloadInfo(bool fullReload);

        IAimpPlayListItem GetItem(int index);

        int GetItemCount();

        IAimpPlayListGroup GetGroup(int index);

        int GetGroupCount();
    }
}