#ifndef __UUICED_SERVERIMP__
#define __UUICED_SERVERIMP__

#include "constdef.h"
#include <uuice.h>
#include <Ice/Ice.h>
#include <IceUtil/UUID.h>
#include <iostream>
#include <fstream>
#include <string>
#include <set> // for session
#include <mysql.h>
#include <GeoIP.h>
#include <errno.h>//errno
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <algorithm>
#include <cctype>

#if 1
#define DEPLOY_UUFILE
#else
#define DEPLOY_ULTRAIE
#endif

namespace bfs = boost::filesystem;

using namespace std;
using namespace UUTOOLBAR;

extern Ice::LoggerPtr plogger;


static const int MAXCMDLEN=2048;
set<string> sidset;
string userroot = "/root/";

/// @todo
char * get_SID(string& email){
  return "";
}

char * get_puredir(string& email){
  return "";
}

char* getcurtime(){
  time_t lt=time(NULL);
  struct tm *ptr;
  ptr=localtime(&lt);
  static char str[80];
  strftime(str,80,"%F %T",ptr);
  return &(str[0]);
}

void string_replace(string& str,const string &needle,const string &s){
  string::size_type lastpos=0, thispos=0;
	int count=0;
  while ((thispos = str.find(needle, lastpos)) != string::npos){
		count++;
    str.replace(thispos, needle.length(), s);
    lastpos = thispos + s.size();
		if(count>1000){
			cout << "Servere ERROR: " << __FUNCTION__	<< endl;
			return;
		}
  }
}

class UserI : public User {
  private:
    MYSQL mysql,*connection;
    MYSQL_RES *res;
    MYSQL_ROW row;
  public:

    UserI():connection(NULL),res(NULL),row(NULL){
      connectDB();
    }
    virtual ~UserI(){
      mysql_close(connection);
    }

    bool connectDB(){
      if (mysql_init(&mysql)){
				mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
        if((connection=mysql_real_connect(&mysql,"localhost",MYSQLDBUSER,MYSQLDBPASS,MYSQLDBNAME,0,MYSQLSOCKET,0))==NULL){
					//mysql_query(&mysql,"SET NAMES 'utf8'");
          cout << mysql_error(&mysql) <<endl;
          return false;
        }else{
          cout << "Connected to mysql!" << endl;
          return true;
        }
      }
      return false;
    }

    LOGINRESPONSE Login(const string& email,const string& password,string& sid,const ::Ice::Current& cur){
      LOGINRESPONSE result=UNKNOWNUSER;
      //plogger->print(cur.con->toString());

      string cur_con_str = cur.con->toString();
      cout << cur_con_str << endl;
      int pos=0,pos2=0;
      string tmp = ((pos=cur_con_str.rfind(" "))==std::string::npos) ? "" : cur_con_str.substr(pos+1,cur_con_str.size()-pos-1);
      if(tmp.empty()) return UNKNOWNUSER;
      string clientip,clientport;
      if((pos2=tmp.find(":")) != std::string::npos){
        clientip = tmp.substr(0,pos2);
        clientport = tmp.substr(pos2+1,tmp.size()-pos2-1);
        cout << "client ip:" << clientip << endl;
        cout << "client port:" << clientport << endl;
      }else{ return UNKNOWNUSER; }


#ifdef DEPLOYMENT
			GeoIP * gi = GeoIP_new(GEOIP_STANDARD);
      string country_code = GeoIP_country_code_by_name(gi,clientip.c_str());
      GeoIP_delete(gi);
      cout << "country where client from:" << country_code << endl;
#else
      string country_code = "CN";
#endif

      try{
        if (!connection && !connectDB()){
          return UNKNOWNUSER;
        }
        res=mysql_store_result(connection);
#ifdef DEPLOY_ULTRAIE
        string cmd = "SELECT password FROM UltraUser where email='";
#else
        string cmd = "SELECT password FROM pw_members where email='";
#endif
        cmd.append(email);
        cmd.append("'");
        mysql_real_query(&mysql,cmd.c_str(),(unsigned int)cmd.size());
        res = mysql_use_result(&mysql);
        cout << email.c_str() << " tried to login !" << endl;
        sid="";
        if(res==NULL){
          cout << "EXCEPTION: Res becomes null!" << endl;
          mysql_close(connection);
          connection=NULL;
          connectDB();
          result=WRONGPASS;//add for no core dump...
        }else{
          if(row = mysql_fetch_row(res)){
            if(strcmp(password.c_str(),row[0])==0){
              sid = IceUtil::generateUUID();
              sidset.insert(sid);
              result = LOGINOK;
            }else{
              cout << "login failed!" << endl;
              result = WRONGPASS;
            }
          }
        }

        mysql_free_result(res);
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
				cout << __FUNCTION__ << " - Unknown error!" << endl;
      }
      return result;
    }


    LOGINRESPONSE LoginUUFile(const string& email,const string& password,string& sid,const ::Ice::Current& cur){
      LOGINRESPONSE result=UNKNOWNUSER;
      //plogger->print(cur.con->toString());

      string cur_con_str = cur.con->toString();
      cout << cur_con_str << endl;
      int pos=0,pos2=0;
      string tmp = ((pos=cur_con_str.rfind(" "))==std::string::npos) ? "" : cur_con_str.substr(pos+1,cur_con_str.size()-pos-1);
      if(tmp.empty()) return UNKNOWNUSER;
      string clientip,clientport;
      if((pos2=tmp.find(":")) != std::string::npos){
        clientip = tmp.substr(0,pos2);
        clientport = tmp.substr(pos2+1,tmp.size()-pos2-1);
        cout << "client ip:" << clientip << endl;
        cout << "client port:" << clientport << endl;
      }else{ return UNKNOWNUSER; }


#ifdef DEPLOYMENT
			GeoIP * gi = GeoIP_new(GEOIP_STANDARD);
      string country_code = GeoIP_country_code_by_name(gi,clientip.c_str());
      GeoIP_delete(gi);
      cout << "country where client from:" << country_code << endl;
#else
      string country_code = "CN";
#endif

      try{
        if (!connection && !connectDB()){
          return UNKNOWNUSER;
        }
        res=mysql_store_result(connection);
        string cmd = "SELECT password FROM pw_members where email='";
        cmd.append(email);
        cmd.append("'");
        mysql_real_query(&mysql,cmd.c_str(),(unsigned int)cmd.size());
        res = mysql_use_result(&mysql);
        cout << email.c_str() << " tried to login !" << endl;
        sid="";
        if(res==NULL){
          cout << "EXCEPTION: Res becomes null!" << endl;
          mysql_close(connection);
          connection=NULL;
          connectDB();
          result=WRONGPASS;//add for no core dump...
        }else{
          if(row = mysql_fetch_row(res)){
            if(strcmp(password.c_str(),row[0])==0){
              sid = IceUtil::generateUUID();
              sidset.insert(sid);
              result = LOGINOK;
            }else{
              cout << "login failed!" << endl;
              result = WRONGPASS;
            }
          }
        }

        mysql_free_result(res);
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
				cout << __FUNCTION__ << " - Unknown error!" << endl;
      }
      return result;
    }
















    bool Logout(const string& sid,const ::Ice::Current& cur){
      if(sidset.find(sid)!=sidset.end()){
        cout << "SessionID "<< sid << " logged out!" << endl;
        sidset.erase(sid);
        return true;
      }else{
        return false;
      }
    }
    bool CreateFolder(const string& foldername,FOLDERPRIV fp,const string& sid,const ::Ice::Current& cur){
      try{
        if(sidset.find(sid)!=sidset.end()){
          //forbidden illegal chars like ".."
          if (foldername.find("..")!=string::npos){
            return false;
          }
          //Create folder for user
          bfs::path folderpath(userroot+foldername,bfs::no_check);
          if(bfs::create_directory(folderpath)){
            //create_directory() no longer throws if the directory already exists.
            //A bool is returned,indicating that the directory did not preexist.
            //Similar changes made to create_directories().
            return true;
          }
          //Folder's access control and password setting
          //TODO
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "CreateFolder Exception!" <<endl;
      }
      return false;
    }
    bool DeleteFileFolder(const string& fname,const string&sid,const ::Ice::Current& cur){
      try{
        if(sidset.find(sid)!=sidset.end()){
          //forbidden illegal chars like ".."
          if (fname.find("..")!=string::npos){
            return false;
          }
          bfs::path fpath(userroot+fname,bfs::no_check);
          if (bfs::exists(fpath)){
            bfs::remove_all(fpath);
            return true;
          }
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "DeleteFolder Exception!" <<endl;
      }
      return false;
    }
    bool RenameFileFolder(const string& oldname,const string& newname,const string& sid,const ::Ice::Current&){
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (oldname.find("..")!=string::npos || newname.find("..")!=string::npos){
            return false;
          }
          bfs::path oldpath(userroot+oldname,bfs::no_check);
          bfs::path newpath(userroot+newname,bfs::no_check);
          if (bfs::exists(oldpath)){
            bfs::rename(oldpath,newpath);
            return true;
          }
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "RenameFolder Exception!" <<endl;
      }
      return false;
    }
    Ice::Long PostUploadFile(const string& _filename,::Ice::Long filesize,const string& desc,
        const string& tag,const string& email,const string& sid,string& fetchurl,const ::Ice::Current& cur)
    {
      Ice::Long fid=-1;
      try{
        string filename = _filename;
        if(sidset.find(sid)!=sidset.end()){
          string oldfilepath = UPLOAD_ROOT+filename;
          bfs::path filepath(oldfilepath,bfs::no_check);
          if (!bfs::exists(filepath)){
            cout << filepath.string().c_str() <<endl;
            cout << "File is not there..."<<endl;
            return false;
          }

          long _fsize = bfs::file_size(filepath);
          cout << "local file size:" << _fsize << "\t incoming size:"<< filesize << endl;
          if (_fsize!=filesize){return false;}
          if (!connection && !connectDB()){ return false; }

					PRINTFUNCLINE

          //1 update UltraUser table
          res=mysql_store_result(connection);
          stringstream sstream;
#ifdef DEPLOY_ULTRAIE
          sstream << "update UltraUser set spaceused=spaceused+" << filesize << " where email='" << email <<"'";
#else
          sstream << "update pw_members set spaceused=spaceused+" << filesize << " where email='" << email <<"'";
#endif
          cout << sstream.str() << endl;
          if(mysql_query(&mysql,sstream.str().c_str())){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
          }
          mysql_free_result(res);

					PRINTFUNCLINE


          //2 get file extension name
          int pos=0;
          string extension = ((pos=filename.rfind("."))==std::string::npos) ? "" : filename.substr(pos+1,filename.size()-pos-1);
          LOWERWSTR(extension);

					PRINTFUNCLINE

          //minus one because we need to erase the dot

          sstream.str(""); // reset stringstream
          sstream << IceUtil::generateUUID();

					PRINTFUNCLINE

          if(!extension.empty())
            sstream << "." << extension;
          string newname(sstream.str());

					PRINTFUNCLINE


          //3 create puredir
          //TODO -  there should have some interface to provide a puredir
          string puredir;
          string DLpassword;
          bool isHTMLFile=true;
          if(extension!="html" && extension!="htm"){
            puredir = DOWNLOAD_ROOT;
            DLpassword = IceUtil::generateUUID();
            isHTMLFile = false;
          }

					PRINTFUNCLINE
					cout << "filename: " << filename.c_str() << endl;
					cout << "email: " << email.c_str() << endl;
					cout << "sid: " << sid.c_str() << endl;

          int defaultDLbandwith=30;
          int defaultstatus=1;

          //4 insert UltraFile table
          sstream.str(""); // reset stringstream
					PRINTFUNCLINE
          //TODO - replace ' with \' for filename
          string_replace(filename,"'","\\'");
					PRINTFUNCLINE
          cout << "filename: " << filename.c_str() << endl;
          sstream << "insert into `UltraFile` (`email`,`ori_filename`,`mod_filename`,`fsize`,`extension`,`puredir`,"
            "`DLpassword`,`DLBandwidth`,`ULtime`,`lastDLtime`,`hits`,`fdesc`,`tag`,`ipaccess`,`status`)"
            " values ('"<<email<<"','"<<filename<<"','"<<newname<<"','"<<filesize<<"','"<<extension<<"','"<<puredir<<
            "','"<<DLpassword<<"',"<<defaultDLbandwith<<",'"<<getcurtime()<<"',NULL,0,'"<<desc<<"','"<<tag<<
            "','*',"<<defaultstatus<<")";
          cout << sstream.str() << endl;
					PRINTFUNCLINE
          res=mysql_store_result(connection);
          if(mysql_query(&mysql,sstream.str().c_str())){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
          }else{
            //5 rename file
            string newfolderpath = isHTMLFile ? HTML_ROOT : puredir.substr(0,puredir.size()-2);
            cout << "new folder:" << newfolderpath << endl;
					  PRINTFUNCLINE
            string newfilename = newfolderpath+newname;
            //bfs::path newfilepath(newfilename,bfs::no_check);
            //bfs::rename(filepath,newfilepath);
            if(rename(oldfilepath.c_str(),newfilename.c_str()) != 0){
              cout << "ERROR: rename " << oldfilepath.c_str() << " to "<< newfilename.c_str() << endl;
							cout << strerror(errno) << endl;
              return false;
            }
					  PRINTFUNCLINE
            if(chmod(newfilename.c_str(),S_IRUSR|S_IRGRP|S_IROTH)!=0){
					    PRINTFUNCLINE
              return false;
            }
            //6 generate fetch url
            sstream.str(""); // reset stringstream
            fid=mysql_insert_id(&mysql);
					  PRINTFUNCLINE
#ifdef DEPLOY_ULTRAIE
            sstream << UltraIEURL << "fetch.php?id=" << fid << std::ends;
#else
            sstream << UUFileURL << fid << ".html" << std::ends;
#endif
            fetchurl=sstream.str();
          }
          mysql_free_result(res);
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
			} catch(...){
        cout << "PostUploadFile Exception!" <<endl;
      }
      return fid;
    }


    bool PostUploadThumb(const string& thumbname,::Ice::Long id,const string& sid,const ::Ice::Current& cur){
      bool result=false;
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);
          //1 move thumb
          bfs::path oldpath(UPLOAD_ROOT+thumbname,bfs::no_check);
          bfs::path newpath(THUMB_ROOT+thumbname,bfs::no_check);
          if (bfs::exists(oldpath) && !bfs::exists(newpath)){
            bfs::copy_file(oldpath,newpath);
            bfs::remove(oldpath);
            //Unix function
            string _npath = THUMB_ROOT+thumbname;
            if(chmod(_npath.c_str(),S_IRUSR|S_IRGRP|S_IROTH)!=0){
              return false;
            }

            // 2. update OnlineBookMark table thumb column
            stringstream sstream;
            sstream << "update OnlineBookmark set thumb='" << thumbname << "' where id="<< id <<"";
            cout << sstream.str() << endl;
            if(mysql_query(&mysql,sstream.str().c_str())){
              cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            }
            mysql_free_result(res);
            result = true;
          }
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){}
      return result;
    }

    bool PostUploadIcon(const string& iconname,::Ice::Long id,const string& sid,const ::Ice::Current& cur){
      bool result=false;
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);
          //1 move thumb
          bfs::path oldpath(UPLOAD_ROOT+iconname,bfs::no_check);
          bfs::path newpath(ICON_ROOT+iconname,bfs::no_check);
          if (bfs::exists(oldpath) && !bfs::exists(newpath)){
            bfs::copy_file(oldpath,newpath);
            bfs::remove(oldpath);
            //Unix function
            string _npath = ICON_ROOT+iconname;
            if(chmod(_npath.c_str(),S_IRUSR|S_IRGRP|S_IROTH)!=0){
              return false;
            }

            // 2. update OnlineBookMark table thumb column
            stringstream sstream;
            sstream << "update OnlineBookmark set icon='" << iconname << "' where id="<< id <<"";
            cout << sstream.str() << endl;
            if(mysql_query(&mysql,sstream.str().c_str())){
              cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            }
            mysql_free_result(res);
            result = true;
          }
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){}
      return result;
    }

    ::Ice::Long UsedSpaceSize(const string& email,const string& sid,const ::Ice::Current& cur){
      ::Ice::Long result=0;
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (connection){
            res=mysql_store_result(connection);
#ifdef DEPLOY_ULTRAIE
            string cmd = "SELECT spaceused FROM UltraUser where email='";
#else
            string cmd = "SELECT spaceused FROM pw_members where email='";
#endif
            cmd.append(email).append("'");
						cout<< __FUNCTION__ << cmd.c_str() << endl;
            mysql_real_query(&mysql,cmd.c_str(),(unsigned int)cmd.size());
            res = mysql_use_result(&mysql);
            if(res==NULL){
              cout << "EXCEPTION2: Res becomes null!" << endl;
              mysql_close(connection);
              connection=NULL;
              connectDB();
              result=0;//add for no core dump...
            }else{
              if(row = mysql_fetch_row(res)){
                result = atoi(row[0]);
              }
            }
            mysql_free_result(res);
          }
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
			} catch(...){
        cout << "UsedSpaceSize Exception!" <<endl;
      }
      return result;
    }
    ::Ice::Long SaveOnlineBookmark(const string& url,const string& title,const string& desc,const string& tag,const string& email,const string& sid,const ::Ice::Current&){
      ::Ice::Long result=0;
      try{
				//string _title(title);
        //string_replace(_title,"'","\\'");
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);
          char cmd[MAXCMDLEN];
          snprintf(cmd,MAXCMDLEN,"insert into `OnlineBookmark` (`email`,`url`,`title`,`description`,`tag`,`icon`,`iconURL`,`thumb`,`folder`,`addtime`) values ('%s','%s','%s','%s','%s','','','','',%s)",
              //email.c_str(),url.c_str(),_title.c_str(),desc.c_str(),tag.c_str(),"NOW()");
              email.c_str(),url.c_str(),title.c_str(),desc.c_str(),tag.c_str(),"NOW()");
          if(mysql_query(&mysql,cmd)){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            mysql_free_result(res);
            return 0;
          }
          result=mysql_insert_id(&mysql);
          cout << cmd << ":\t" << result <<endl;
          mysql_free_result(res);
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "SaveFavLink Exception!" <<endl;
      }
      return result;
    }
    bool GetOLBMURLs(const string& email,const string& sid,IEFavURLs& existingOLBMURLs,const ::Ice::Current&){
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);

          char cmd[MAXCMDLEN+1]={};
          snprintf(cmd,MAXCMDLEN,"select url from `OnlineBookmark` where email='%s'",email.c_str());
          if(mysql_query(&mysql,cmd)){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            if(res)
              mysql_free_result(res);
            return false;
          }
          cout << cmd <<endl;
          if(!(res=mysql_store_result(&mysql))){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            return false;
          }
          MYSQL_ROW row;
          while (row = mysql_fetch_row(res)) {
            string url=row[0];
            existingOLBMURLs.push_back(url);
          }
          mysql_free_result(res);
        }else{
        	return false;
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "GetOLBMURLs Exception!" <<endl;
      }
      return true;
    }
    bool UploadImgSeq(const ImgSeq& fseq,const string& email,const string& sid,UPLOADFILETYPE ftype,Ice::Long fid,const ::Ice::Current&){
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);

          vector<Image>::const_iterator begin=fseq.begin();
          vector<Image>::const_iterator end=fseq.end();
          for(vector<Image>::const_iterator i=begin;i!=end;++i){
            string fpath;
            switch(ftype){
              case MYICON:
                fpath = ICON_ROOT + i->name;
                break;
              case MYTHUMB:
                fpath = THUMB_ROOT + i->name;
                break;
            }
            cout << fpath.c_str() << endl;
            ofstream outfile(fpath.c_str(),ofstream::binary);
            outfile.write((char*)&i->data[0],i->data.size());
            outfile.close();

            stringstream sstream;
            sstream << "update UltraFile set thumb='" << i->name << "' where id="<< fid <<"";
            cout << sstream.str() << endl;
            if(mysql_query(&mysql,sstream.str().c_str())){
              cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            }
          }
          mysql_free_result(res);
        }else{
        	return false;
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "UploadImgSeq Exception!" <<endl;
        return false;
      }
      return true;
    }
    bool UploadOLBM(const OLBMItems& items,const string& email,const string& sid,const ::Ice::Current&){
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);
          map<string,OLBMValue>::const_iterator begin= items.begin();
          map<string,OLBMValue>::const_iterator end= items.end();
          for(map<string,OLBMValue>::const_iterator i=begin;i!=end;++i){
            string url=(*i).first;
            string title=(*i).second.title;
            string_replace(title,"'","\\'");
            string desc=(*i).second.description;
            string tag=(*i).second.tag;
            string folder=(*i).second.folder;
            //string iconpath=(*i).second.iconpath;
            string iconpath="";
            string iconURL=(*i).second.iconURL;

            char cmd[MAXCMDLEN];
            snprintf(cmd,MAXCMDLEN,"insert ignore into `OnlineBookmark` (`email`,`url`,`title`,`description`,`tag`,`icon`,`iconURL`,`thumb`,`folder`,`addtime`) values ('%s','%s','%s','%s','%s','%s','%s','','%s',%s)",
                email.c_str(),url.c_str(),title.c_str(),desc.c_str(),tag.c_str(),iconpath.c_str(),iconURL.c_str(),folder.c_str(),"NOW()");
            if(mysql_query(&mysql,cmd)){
              cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
              //mysql_free_result(res);
              //return false;
            }
            //result=mysql_insert_id(&mysql);
            cout << cmd <<endl;
          }
          mysql_free_result(res);
        }else{
        	return false;
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "UploadOLBM Exception!" <<endl;
      }
      return true;
    }
    bool DownloadOLBM(const string& email,const string& sid,OLBMItems& items,const ::Ice::Current&){
      try{
        if(sidset.find(sid)!=sidset.end()){
          if (!connection && !connectDB()){ return false; }
          res=mysql_store_result(connection);
          char cmd[MAXCMDLEN];
          snprintf(cmd,MAXCMDLEN,"select url,title,iconURL,folder from `OnlineBookmark` where email='%s'",email.c_str());
          if(mysql_query(&mysql,cmd)){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            mysql_free_result(res);
            return false;
          }
          cout << cmd <<endl;
          if(!(res=mysql_store_result(&mysql))){
            cout << mysql_errno(&mysql) << ": " << mysql_error(&mysql)<< endl;
            return false;
          }
          MYSQL_ROW row;
          while (row = mysql_fetch_row(res)) {
            string url=row[0];
            string title=row[1];
            string iconURL=row[2];
            string folder=row[3];

            OLBMValue olbmVal;
            olbmVal.title=title;
            olbmVal.iconURL=iconURL;
            olbmVal.folder=folder;

            items[url]=olbmVal;
          }
          mysql_free_result(res);
        }else{
        	return false;
        }
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "DownloadOLBM Exception!" <<endl;
      }
      return true;
    }
    bool GetFTPInfoForUpload(const string& email,string& ip,::Ice::Short& port,const ::Ice::Current&){
#ifdef DEPLOY_ULTRAIE
      try{
        if (!connection && !connectDB()){ return false; }
        stringstream sstream;
        sstream <<"select ULIP from UltraUser where email='" << email <<"'";
        cout << sstream.str() << endl;
        mysql_real_query(&mysql,sstream.str().c_str(),(unsigned int)sstream.str().size());
        res = mysql_use_result(&mysql);
        if(res==NULL){
          cout << "EXCEPTION3: Res becomes null!" << endl;
          mysql_close(connection);
          connection=NULL;
          connectDB();
          return false;
        }else{
          if(row = mysql_fetch_row(res)){
            ip  = row[0];
            port= 21; // default ftp port for all ftp servers is 21.
          }
        }
        mysql_free_result(res);
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "GetFTPInfoForUpload Exception!" <<endl;
      }
#else
			ip = "66.235.160.14";
			port = 21;
#endif
      return true;
    }

    // TODO(fuheng)
    string IsLatestVersion(const string& version,const ::Ice::Current&){
      if(version.compare("1.0.0.49")==0){
				return string();
			}else{
				return string("http://www.ultraie.com/UltraIE_Installer.exe");
			}
    }

    string IsFileUploaded(const string& _filename,Ice::Long fsize,const ::Ice::Current&){
      string fetchurl;
      try{
				string filename(_filename);
        string_replace(filename,"'","\\'");
        if (!connection && !connectDB()){ return false; }
        stringstream sstream;
        sstream <<"select id from UltraFile where ori_filename='" << filename <<"' and fsize="<<fsize;
        cout << sstream.str() << endl;
        mysql_real_query(&mysql,sstream.str().c_str(),(unsigned int)sstream.str().size());
        res = mysql_use_result(&mysql);
        if(res==NULL){
          cout << "EXCEPTION3: Res becomes null!" << endl;
          mysql_close(connection);
          connection=NULL;
          connectDB();
          return false;
        }else{
          //if(mysql_num_rows(res)!=0){
            string fid;
            if(row = mysql_fetch_row(res)){
              fid = row[0];
              stringstream sstream;
#ifdef DEPLOY_ULTRAIE
	            sstream << UltraIEURL << "fetch.php?id=" << fid << std::ends;
#else
  	          sstream << UUFileURL << fid << ".html" << std::ends;
#endif
              fetchurl=sstream.str();
            }
          //}
        }
        mysql_free_result(res);
      }catch(const Ice::Exception& e){
				cout << __FUNCTION__ << " - Unexpected run-time error: " << e << endl;
      }catch(...){
        cout << "UsedSpaceSize Exception!" <<endl;
      }
      return fetchurl;
    }

};

#endif
