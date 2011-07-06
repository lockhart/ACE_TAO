/**
 *  @file   ServantContainerInterceptorRegistration_Impl.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
#if !defined SERVANTCONTAINERINTERCEPTORREGISTRATION_IMPL_H
#define SERVANTCONTAINERINTERCEPTORREGISTRATION_IMPL_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Generic/Generic_Container_export.h"

#include "ccm/Generic/CCM_ServantContainerInterceptorC.h"
#include "ccm/Generic/CCM_ServantContainerInterceptorRegistrationC.h"
#include "ciao/Containers/Generic/Interceptor_Registration_T.h"

namespace CIAO
{
  /**
   * @class ServantContainerInterceptorRegistration_Impl
   *
   * @brief Implementation of the CCM_ServantContainerInterceptorRegistration interface
   *
   * Implementation of the registration process of Servant Container
   * Interceptors
   *
   * Part of COPI (Container Portable Interceptor)
   */
  class GENERIC_CONTAINER_Export ServantContainerInterceptorRegistration_Impl
    : public Interceptor_Registration_T<
        ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration,
        ::Components::ContainerPortableInterceptor::ServantContainerInterceptor>
  {
  public:
    ServantContainerInterceptorRegistration_Impl (void);
    virtual ~ServantContainerInterceptorRegistration_Impl (void);

    /// Registers a ServantContainerInterceptor.
    /// Returns a cookie for this registration.
    virtual ::Components::Cookie *
    register_servant_interceptor (
      ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_ptr ci);

    /// Unregisters a ServantContainerInterceptor.
    /// Throws an InvalidRegistration exception when the
    /// ServantContainerInterceptor cannot be found.
    /// Returns the ServantContainerInterceptor
    virtual ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_ptr
    unregister_servant_interceptor (::Components::Cookie * cookie);
  };
}

#include /**/ "ace/post.h"

#endif /* CCM_SERVANTCONTAINERINTERCEPTORREGISTRATION_IMPL_H */