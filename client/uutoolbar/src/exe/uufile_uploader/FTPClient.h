#ifndef INC_FTPCLIENT_H
#define INC_FTPCLIENT_H

// STL-includes
#include <memory>
#include <queue>

// other includes
#include "../Stub_Skeleton/6bees_socket.h"
#include "FTPDataTypes.h"
//#include "FTPFileState.h"

enum SUSPENDSTATUS {NO_SUSPEND,SUSPEND_SUCCESS,SUSPEND_RECOVER};

////////////////////////////////////////////////////////////////////////////////
/// Namespace for all FTP-related classes.
////////////////////////////////////////////////////////////////////////////////
namespace nsFTP{
  typedef std::vector<std::wstring> TStringVector;
  using namespace _6bees_socket;

  class ITransferNotification{
  public:
    virtual void OnBytesReceived(
      const TByteVector& /*vBuffer*/,
      long /*lReceivedBytes*/) {}
    virtual void OnPreBytesSend(TByteVector& /*vBuffer*/,
      size_t& /*bytesToSend*/) {}
  };

  /// @brief FTP client class
  ///
  /// Use this class for all the ftp-client stuff such as
  /// - logon server
  /// - send and receive data
  /// - get directory listing
  /// - ...
  ///
  class CFTPClient{
  public:
    class CNotification;
    class TObserverSet:
      public nsHelper::CObserverBase<CNotification*, TObserverSet*>
    {};

    CFTPClient(
      IBlockingSocket* pSocket=_6bees_socket::BI(),
      unsigned int uiTimeout=10,
      unsigned int uiBufferSize=2048,
      unsigned int uiResponseWait=0);
    virtual ~CFTPClient();

    void AttachObserver(CNotification* pObserver);
    void DetachObserver(CNotification* pObserver);

    bool IsConnected();
    bool IsTransferringData();
    void SetResumeMode(bool fEnable=true);

    bool Login(const CLogonInfo& loginInfo);
    int Logout();

    bool DownloadFile(const std::wstring& strRemoteFile,
      const std::wstring& strLocalFile,
      const CRepresentation& repType=CRepresentation(CType::Image()),
      bool fPasv=false);
    bool UploadFile(const std::wstring& strLocalFile,
      const std::wstring& strRemoteFile,
      bool fStoreUnique=false,
      const CRepresentation& repType=CRepresentation(CType::Image()),
      bool fPasv=false);
    int ChangeWorkingDirectory(const std::wstring& strDirectory);//need by login()
    int Passive(ULONG& ulIpAddress, unsigned short& ushPort);
    int DataPort(const std::wstring& strHostIP, unsigned short ushPort);
    int Abort();
    int RepresentationType(const CRepresentation& repType, DWORD dwSize=0);
    int Restart(DWORD dwPosition);
    int FileSize(const std::wstring& strPath, long& lSize);

    void SetItem(int nItem);
    void SetThreadID(int);
    void SetFL(CONST CString&);
    void Setufilesize(long);
    void SetTimer(nsHelper::CTimer*);
    void SetTerminatethisthread(bool);

  protected:
    bool ExecuteDatachannelCommand(const CDatachannelCmd& crDatachannelCmd,
      const std::wstring& strPath,
      const CRepresentation& representation,
      bool fPasv,
      DWORD dwByteOffset,
      ITransferNotification* pObserver);
    TObserverSet& GetObservers();

  private:
    bool TransferData(const CDatachannelCmd& crDatachannelCmd,
      ITransferNotification* pObserver,
      IBlockingSocket& sckDataConnection);
    bool OpenActiveDataConnection(IBlockingSocket& sckDataConnection,
      const CDatachannelCmd& crDatachannelCmd,
      const std::wstring& strPath,
      DWORD dwByteOffset);
    bool OpenPassiveDataConnection(IBlockingSocket& sckDataConnection,
      const CDatachannelCmd& crDatachannelCmd,
      const std::wstring& strPath,
      DWORD dwByteOffset);
    bool SendData(ITransferNotification* pObserver,
      IBlockingSocket& sckDataConnection);
    bool ReceiveData(ITransferNotification* pObserver,
      IBlockingSocket& sckDataConnection);

    std::wstring GetCmdString(const CDatachannelCmd& crDatachannelCmd,
      const std::wstring& strPath);
    int SimpleErrorCheck(const CReply& Reply);

    bool SendCommand(const std::wstring& strCommand);
    bool SendCommand(const std::wstring& strCommand, CReply& Reply);
    bool GetResponse(CReply& Reply);
    bool GetSingleResponseLine(std::wstring& strResponse);

    bool OpenControlChannel(const std::wstring& strServerHost,
      unsigned short ushServerPort=FTP_PORT_DEFAULT);
    void CloseControlChannel();

    void ReportError(const std::wstring& strErrorMsg,
      const std::string& strFile, DWORD dwLineNr);
    bool GetIpAddressFromResponse(const std::wstring& strResponse,
      ULONG& ulIpAddress, unsigned short& ushPort);

  private:
    ///< timeout for socket-functions
    const unsigned int mc_uiTimeout;

    ///< sleep time between receive calls to socket when getting the response
    const unsigned int mc_uiResponseWait;

    //< end-of-line sequence of current operating system
    const std::wstring mc_strEolCharacterSequence;

    //< buffer for sending and receiving
    mutable TByteVector m_vBuffer;

    //< buffer for server-responses
    mutable std::queue<std::string> m_qResponseBuffer;

    //< representation currently set
    mutable std::auto_ptr<CRepresentation> m_apCurrentRepresentation;

    //TODO -
    //< socket for connection to ftp-server
    std::auto_ptr<IBlockingSocket> m_apSckControlConnection;
    std::auto_ptr<IBlockingSocket> apSckDataConnection;

    //< try to resume download/upload if possible
    bool m_fResumeIfPossible;

    //< list of observers, which are notified about particular actions
    TObserverSet m_setObserver;

    // simonwoo added code here.

    //< the item number which the ftpclient connected whith
    int m_nItem;

    int m_threadID;
    
    CString m_flocation;
    
    long m_ufilesize;
    
    nsHelper::CTimer* p_timer;

    bool terminatethisthread;

    // if this file is already uploaded, this is the length of uploaded file (bytes)
    long lRemoteFileSize;

  public:
    bool ftpalive;

    ///< if true, a file transfer is in progress
    mutable bool m_fTransferInProgress;

    //< indicates that a running filetransfer should be canceled
    mutable bool m_fAbortTransfer;

    SUSPENDSTATUS suspendit;
  };

  /// @brief interface for notification
  ///
  /// Derive your class from this base-class and register this class on CFTPClient.
  /// For example you can use this for logging the sended and received commands.
  class CFTPClient::CNotification:
    public nsHelper::CObserverBase<CFTPClient::TObserverSet*,CFTPClient::CNotification*>
  {
  public:
    virtual void OnInternalError(const std::wstring& /*strErrorMsg*/,
      const std::wstring& /*strFileName*/,
      DWORD /*dwLineNr*/) {}

    virtual void OnBeginReceivingData() {}
    virtual void OnEndReceivingData(long /*lReceivedBytes*/) {}
    virtual void OnBytesReceived(const TByteVector& /*vBuffer*/,
      long /*lReceivedBytes*/) {}
    virtual void OnBytesSent(const TByteVector& /*vBuffer*/,
      long /*lSentBytes*/) {}
    virtual void OnBytesSent2(long lSentBytes,
      CString filelocation,
      int threadID,
      int speed,
      int a,
      int b) {}
    virtual void OnSetLRemoteFileSize(int threadID,
      long lRemoteFileSize){}

    virtual void OnPreReceiveFile(const std::wstring& /*strSourceFile*/,
      const std::wstring& /*strTargetFile*/,
      long /*lFileSize*/) {}
    virtual void OnPreSendFile(const std::wstring& /*strSourceFile*/,
      const std::wstring& /*strTargetFile*/,
      long /*lFileSize*/) {}
    virtual void OnPostReceiveFile(const std::wstring& /*strSourceFile*/,
      const std::wstring& /*strTargetFile*/,
      long /*lFileSize*/) {}
    virtual void OnPostSendFile(const std::wstring& /*strSourceFile*/,
      const std::wstring& /*strTargetFile*/,
      long /*lFileSize*/) {}

    virtual void OnSendCommand(const std::wstring& /*strCommand*/) {}
    virtual void OnResponse(const CReply& /*Reply*/) {}
  };

}


#endif // INC_FTPCLIENT_H