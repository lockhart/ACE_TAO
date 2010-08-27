// -*- C++ -*-
//
// $Id$

#include "CFTLE_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define MIN_ITERATION 2
#define MAX_ITERATION 5

#define SAMPLES_PER_KEY 2

namespace CIAO_CFTLE_Test_Receiver_Impl
{
  //============================================================
  // ContentFilteredTopicListenEventTest_Listener_exec_i
  //============================================================
  ContentFilteredTopicListenEventTest_Listener::ContentFilteredTopicListenEventTest_Listener (
                                              ACE_Thread_ID &thread_id,
                                              Atomic_Long &samples_received)
    : thread_id_ (thread_id),
      samples_received_ (samples_received)
  {
  }

  ContentFilteredTopicListenEventTest_Listener::~ContentFilteredTopicListenEventTest_Listener (void)
  {
  }

  void
  ContentFilteredTopicListenEventTest_Listener::on_one_data (
                                  const ContentFilteredTopicListenEventTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
  {
    ACE_Thread_ID t_id;
    this->thread_id_ = t_id
    ++this->samples_received_;
    ACE_DEBUG ((LM_DEBUG, "ContentFilteredTopicListenEventTest_Listener::on_one_data: "
                          "key <%C> - iteration <%d>\n",
                          an_instance.key.in (),
                          an_instance.iteration));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ContentFilteredTopicListenEventTest_Listener::on_one_data: "
                              "instance handle seems to be invalid "
                              "key <%C> - iteration <%d>\n",
                              an_instance.key.in (),
                              an_instance.iteration));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ContentFilteredTopicListenEventTest_Listener::on_one_data: "
                              "source timestamp seems to be invalid (nil) "
                              "key <%C> - iteration <%d>\n",
                              an_instance.key.in (),
                              an_instance.iteration));
      }
    if (an_instance.iteration <= MIN_ITERATION ||
        an_instance.iteration  > MAX_ITERATION )
      {
        ACE_ERROR ((LM_ERROR, "ContentFilteredTopicListenEventTest_Listener::on_one_data: "
                              "ERROR: Unexpected iteration received.\n"));
      }
  }

  void
  ContentFilteredTopicListenEventTest_Listener::on_many_data (
                                  const ContentFilteredTopicListenEventTestSeq & /* an_instance */,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : thread_id_listener_ (0, 0),
      iterations_ (10),
      keys_ (5),
      samples_expected_ (keys_ * SAMPLES_PER_KEY),
      samples_received_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc =
        this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  ::CFTLE_Test::ContentFilteredTopicListenEventTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ContentFilteredTopicListenEventTest_Listener (
                this->thread_id_listener_,
                this->samples_received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;

    this->samples_expected_ = SAMPLES_PER_KEY * this->keys_;
  }

  void
  Receiver_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CFTLE_Test::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    ::CFTLE_Test::ContentFilteredTopicListenEventTestConnector::Reader_var reader =
      this->context_->get_connection_info_listen_data ();
    if (CORBA::is_nil (reader.in ()))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::configuration_complete - "
                              "Error get reader connection.\n"));
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::ccm_activate - "
                          "Start listening.\n"));
    start ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    char ccm_buf [65];
    ACE_Thread_ID ccm_thread_id;
    ccm_thread_id.to_string (ccm_buf);

    char list_buf [65];
    this->thread_id_listener_.to_string (list_buf);


    if (this->thread_id_listener_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: "
                              "Thread ID for ReaderListener not set!\n"));
      }
    #if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_DEBUG ((LM_DEBUG, "CFT_EVENT: "
                              "Thread switch for ReaderListener seems OK. "
                              "(DDS uses the CCM thread for its callback) "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: CFT_EVENT: "
                              "Thread switch for ReaderListener "
                              "doesn't seem to work! "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #else
    else if (this->thread_id_listener_ == ccm_thread_id)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: CFT_EVENT: ReaderListener: "
                              "DDS seems to use a CCM thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "CFT_EVENT: ReaderListener: "
                              "DDS seems to use its own thread for its callback: "
                              "listener <%C> - component <%C>\n",
                              list_buf,
                              ccm_buf));
      }
    #endif
    if (this->samples_received_ != this->samples_expected_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: CFT_EVENT: ReaderListener: "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_.value ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "CFT_EVENT: ReaderListener: "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_.value ()));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTLE_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
