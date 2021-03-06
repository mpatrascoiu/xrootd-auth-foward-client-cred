#ifndef __XROOTD_AUTH_FORWARD_CLIENT_ID_HH__
#define __XROOTD_AUTH_FORWARD_CLIENT_ID_HH__

#include <XrdAcc/XrdAccAuthorize.hh>
#include <XrdAcc/XrdAccPrivs.hh>
#include <XrdSec/XrdSecEntity.hh>
#include <XrdSecsss/XrdSecsssID.hh>
#include <XrdSys/XrdSysLogger.hh>
#include <XrdVersion.hh>
#include <stdio.h>
#include <sys/types.h>
#include <string>

typedef XrdAccAuthorize *(*GetAuthObject_t)(XrdSysLogger *lp,
                                            const char   *cfn,
                                            const char   *parm);

class AuthForwardClientId : public XrdAccAuthorize
{

public:
  AuthForwardClientId(XrdSysLogger *logger, const char *config, const char *param);
  virtual ~AuthForwardClientId(void);

  XrdAccPrivs Access(const XrdSecEntity    *entity,
                     const char            *path,
                     const Access_Operation oper,
                     XrdOucEnv             *env=0);

  virtual int Audit(const int              accok,
                    const XrdSecEntity    *entity,
                    const char            *path,
                    const Access_Operation oper,
                    XrdOucEnv             *env=0) { return 0; }

  virtual int Test(const XrdAccPrivs      priv,
                   const Access_Operation open) { return 0; };

private:
  const char* getDelegateAuthLibPath(const char *config);
  void loadDelegateAuthLib(const char *libPath);

  // getsssRegistry() attempts to retrieve an existing SSS Registry
  //                  or creates it otherwise
  XrdSecsssID  *getsssRegistry();
  // generatePssIDfromEntity() creates the same user ID the PSS component will use.
  //                           The ID is generated from the sec entity tident field.
  const char   *generatePssIDfromTraceIdent(const char *tident);
  XrdSecEntity *copySecEntity(const XrdSecEntity *entity,
                              const char *pName);

  void *mDelegateAuthLibHandle;
  GetAuthObject_t mAuthObjHandler;
  XrdAccAuthorize *mDelegateAuthLib;

  XrdSecsssID  *mSssRegistry;
  XrdSysLogger *mLogger;
  const char   *mConfig;
  const char   *mParam;
};

#endif  // __XROOTD_AUTH_FORWARD_CLIENT_ID_HH__
