// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// ICE file
module UUTOOLBAR {
    // for image file
    sequence<byte> Imgdata;
    struct Image{
        string name;
        Imgdata data;
    };
    sequence<Image> ImgSeq;

    //onlinebookmark item
    struct OLBMValue{
        string title;
        string description;
        string tag;
        string folder;
        string iconURL;//the original icon URL from the website
        //string iconpath;//local path - todo - to be deleted
    };

    //URL -> OLBMValue
    dictionary<string,OLBMValue> OLBMItems;
    sequence<string> IEFavURLs;

    enum FOLDERPRIV{ ANYONECAN, FRIENDCAN, ONLYSELF , OTHERSNEEDPASS };
    enum LOGINRESPONSE{ UNKNOWNUSER, WRONGPASS, LOGINOK , ACCOUNTDISABLED };
    enum UPLOADFILETYPE{ MYICON, MYTHUMB };

    interface User {
        LOGINRESPONSE Login(string email, string password, out string sid);
        LOGINRESPONSE LoginUUFile(string email, string password, out string sid);
        bool Logout(string sid);

        bool CreateFolder(string foldername,FOLDERPRIV fp,string sid);
        bool DeleteFileFolder(string foldername,string sid);
        bool RenameFileFolder(string oldname,string newname,string sid);

        long PostUploadFile(string filename,long filesize,string crc32,string desc,string tag,string email,string sid,out string fetchurl);

        //long UsedSpaceSize(string email,string sid);
       bool GetUserInfo(string email,out long spaceused,out long ULnum); 

        bool UploadImgSeq(ImgSeq imgs,string email,string sid,UPLOADFILETYPE type,long id);

        // Online Bookmark 
        //return last inserted id
        long SaveOnlineBookmark(string url,string title,string desc,string tag,string email,string sid);

        bool GetOLBMURLs(string email,string sid,out IEFavURLs existingOLBMURLs);
        bool UploadOLBM(OLBMItems items,string email,string sid);
        bool DownloadOLBM(string email,string sid,out OLBMItems items);
        ///////////////////////////////////////////////////////////////////////////////////////////////////

        bool PostUploadThumb(string thumbname,long id,string sid);
        bool PostUploadIcon(string iconname,long id,string sid);

        bool GetFTPInfoForUpload(string email,out string ip,out short port);
        string IsLatestVersion(string version);

        /// When we upload a new file, we will call this function first.
        /// Has this file been uploaded successfully before?
        /// @return if uploaded - URL of uploaded file; not uploaded - empty string
        string IsFileUploaded(string crc32,long filesize);
    };
};
