#include "params.hh"
#include "connect.hh"

ROA_Parameters* ROA_Parameters::_instance = 0;
ROA_Factory* ROA_Factory::_instance = 0;

ROA_Parameters::ROA_Parameters()
  : using_threads_(0),
    thread_flags_(THR_NEW_LWP),
    context_p_(0),
    upcall_(0),
    forwarder_(0),
    oa_(0)
{
}

ROA_Parameters*
ROA_Parameters::instance()
{
  if (_instance == 0)
    {
      _instance = new ROA_Parameters;
    }
  return _instance;
}

ROA_Factory*
ROA_Factory::instance()
{
  if (_instance == 0)
    {
      _instance = new ROA_Factory;
    }
  return _instance;
}

// Determine the appropriate default thread flags, based on system.
// When I put the concurrency strategy into the factory, then this will
// go away b/c the concurrency strategy will do this appropriate
// for each platform!
#  if defined(linux)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(WIN32)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  elif defined(sparc)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  else
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  endif

ROA_Factory::CONCURRENCY_STRATEGY*
ROA_Factory::concurrency_strategy()
{
  ROA_Parameters* p = ROA_Parameters::instance();


  if (p->using_threads())
    {
      // Set the strategy parameters
      threaded_strategy_.open(ACE_Service_Config::thr_mgr(), ROA_DEFAULT_THREADFLAGS);
      concurrency_strategy_ = &threaded_strategy_;
    }
  else
    {
      concurrency_strategy_ = 0;
    }
}


#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Thread_Strategy<ROA_Handler>;
template class ACE_Concurrency_Strategy<ROA_Handler>;
template class ACE_Creation_Strategy<ROA_Handler>;
template class ACE_Scheduling_Strategy<ROA_Handler>;
template class ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR>;
#endif
