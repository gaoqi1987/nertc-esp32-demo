#ifndef __NERTC_SDK_ERROR_H__
#define __NERTC_SDK_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

// Error Code define
typedef enum {
  /* No Error */
  NERTC_SDK_ERR_SUCCESS                                   = 0,

  /** 未通过服务器权限校验 */
  NERTC_SDK_ERR_SERVER_PERMISSION_DENIED                  = 403, 
   
  /** 请求服务器超时 */      
  NERTC_SDK_ERR_SERVER_REQUEST_TIMEOUT                    = 408,        

  /** 服务器请求参数错误 */
  NERTC_SDK_ERR_SERVER_INVALID_PARAMETERS                 = 414,  
  
  /** 非法的APP KEY */
  NERTC_SDK_ERR_SERVER_INVALID_APP_KEY                    = 417,   
  
  /** 服务器未知错误 */
  NERTC_SDK_ERR_SERVER_UNKNOWN_ERROR                      = 500,           

  /** 权限秘钥错误*/
  NERTC_SDK_ERR_SERVER_PERMISSION_KEY_ERROR               = 901,  
  
  /** 权限秘钥过期*/
  NERTC_SDK_ERR_SERVER_PERMISSION_KEY_TIMEOUT             = 902,   

  /* General error (no specified reason). */
  NERTC_SDK_ERR_FATAL                                     = 30001,
  
  /** 内存耗尽 */
  NERTC_SDK_ERR_OUT_OF_Of_MEMORY                          = 30002,            

  /* Invalid parameter */
  NERTC_SDK_ERR_INVALID_PARAMETER                         = 30003,

  /** 不支持的操作 */
  NERTC_SDK_ERR_NOT_SUPPORT                               = 30004,                 

  /* Invalid state */
  NERTC_SDK_ERR_INVALID_STATE                             = 30005,

  /** 用户未找到 */
  NERTC_SDK_ERR_USER_NOT_FOUND                            = 30105,       

  /** 媒体连接断开 */
  NERTC_SDK_ERR_MEDIA_DISCONNECTED                        = 30204, 

  /** 信令连接断开 */
  NERTC_SDK_ERR_SIGNAL_DISCONNECTED                       = 30205,
  
  /** 被踢出房间 */
  NERTC_SDK_ERR_KICKED_BY_SERVER                          = 30206,             

  /** 房间已被关闭 */
  NERTC_SDK_ERR_KICKED_BY_CHANNEL_CLOSED                  = 30207,         

} nertc_sdk_error_code_e;

#ifdef __cplusplus
}
#endif

#endif // __NERTC_SDK_ERROR_H__