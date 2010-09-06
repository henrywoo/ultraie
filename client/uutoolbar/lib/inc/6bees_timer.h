#ifndef __6BEE_TIMER__
#define __6BEE_TIMER__

namespace _6bees_timer{

  /// @brief Calculates elapsed CPU time.
  /// @brief Is useful for calculating transfer rates.
  class CTimer{
    enum {kNumber=1024};
  public:
    CTimer(){
      init();
    }

    /// Start the timer.
    //void Start(){
    //  m_iniStart = clock();///CLOCKS_PER_SEC;
    //}

    /// Restart the timer.
    void Restart(){
      session_start_time_ = clock();
      session_sent_bytes_ = 0;
    }

    //void ReLaunch(){
    //  init();
    //}

    void init(){
      session_start_time_ = clock();
      session_sent_bytes_ = 0;
      m_timeremaining = 0;
      m_size_long = -1;
      session_speed_ = 1;
      m_finishedbytes = 0;
    }

    void addSentBytes(long a){
      session_sent_bytes_ += a;
    }

    long getSpeed(){
#if 0
      long a = GetElapsedTime();
      if(a > m_bound && a!=0 ){
        session_speed_=(long)(session_sent_bytes_*CLOCKS_PER_SEC/(a*kNumber));
        return (session_speed_==0 ? 1 : session_speed_);
      }else{
        return -1;
      }
#else
      long session_time_passed_ = GetElapsedTime();
      if(session_time_passed_){
        /// @note: integer over flow
        /// session_speed_ = session_sent_bytes_*CLOCKS_PER_SEC/(session_time_passed_*kNumber);
        session_speed_ = (long)(session_sent_bytes_/(session_time_passed_*(kNumber/CLOCKS_PER_SEC)));
      }
      return session_speed_>0 ? session_speed_ : 1;
#endif
    }

    // by second
    long GetTimepassed(){
      //return (clock()-m_iniStart)/CLOCKS_PER_SEC;
      return (clock() - session_start_time_)/CLOCKS_PER_SEC;
    }

    // by second
    long GetRemainingTime(){
      if (session_speed_!=0 && m_size_long!=-1 && m_size_long>m_finishedbytes){
        m_timeremaining = (m_size_long - m_finishedbytes)/(session_speed_*kNumber);
      }
      return m_timeremaining;
    }

    void SetFileSize(long _fsize){
      m_size_long = _fsize;
    }

  private:
    /// Elapsed CPU time for process (start of measurement)
    clock_t session_start_time_;// in session range
    //clock_t m_iniStart;// in life of this object

    long   m_timeremaining;
    long   session_speed_; // KBytes/s
    long   m_size_long; // whole file size

    /// need to reset continuously, sent bytes in one session
    long   session_sent_bytes_;

    

    /// Get the elapsed time in millionseconds.
    long GetElapsedTime() const{
      return clock() - session_start_time_;// +1 is to make sure it is not equal to 0
    }

  public:

    ///  the total bytes sent out
    long m_finishedbytes;
  };

};

#endif