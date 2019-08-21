//
//  BgLoginViewModel.h
//  zhonghe
//
//  Created by 韩旭 on 2017/9/7.
//  Copyright © 2017年 韩旭. All rights reserved.
//
#ifndef dispatch_main_async_safe(block)
    #define dispatch_main_async_safe(block)\
    if ([NSThread isMainThread]) {\
    block();\
    } else {\
    dispatch_async(dispatch_get_main_queue(), block);\
    }
#endif
typedef void (^HTDownloadProgress)(int64_t bytesRead,
                                    int64_t totalBytesRead);

typedef HTDownloadProgress HTGetProgress;
typedef HTDownloadProgress HTPostProgress;

typedef void (^HTUploadProgress)(int64_t bytesWritten,
                                  int64_t totalBytesWritten);

typedef NS_ENUM(NSUInteger, HTResponseType) {
    kHTResponseTypeJSON = 1, // 默认
    kHTResponseTypeXML  = 2, // XML
    kHTResponseTypeData = 3  //
};

typedef NS_ENUM(NSUInteger, HTRequestType) {
    kHTRequestTypeJSON = 1,       // 默认
    kHTRequestTypePlainText  = 2  // 普通text/html
};

typedef NS_ENUM(NSInteger, HTNetworkStatus) {
    kHTNetworkStatusUnknown          = -1,  //未知网络
    kHTNetworkStatusNotReachable     = 0,   //网络无连接
    kHTNetworkStatusReachableViaWWAN = 1,   //2，3，4G网络
    kHTNetworkStatusReachableViaWiFi = 2,   //WIFI网络
};

/**
 *  所有的接口返回值均为NSURLSessionTask
 */
typedef NSURLSessionTask HTURLSessionTask;

/*!
 *
 *  请求成功的回调
 *
 *  @param response 服务端返回的数据类型
 */
typedef void(^HTResponseSuccess)(id response);

/*!
 *
 *  网络响应失败时的回调
 *
 */
typedef void(^HTResponseFail)(NSError *error);

/************* class **************/
@interface NSString (MD5)
- (NSString *)md5String;
@end
/************* class **************/

@interface HTNetWorking : NSObject

/*!
 *
 *  用于指定网络请求接口的基础url
 */
+ (void)updateBaseUrl:(NSString *)baseUrl;
+ (NSString *)baseUrl;

/**
 *	设置请求超时时间，默认为30秒
 *
 */
+ (void)setTimeout:(NSTimeInterval)timeout;

/**
 *	当检查到网络异常时，是否从从本地提取数据。默认为NO。一旦设置为YES,当设置刷新缓存时，
 *  若网络异常也会从缓存中读取数据。同样，如果设置超时不回调，同样也会在网络异常时回调，除非
 *  本地没有数据！
 *
 */
+ (void)obtainDataFromLocalWhenNetworkUnconnected:(BOOL)shouldObtain;

/**
 *
 *	默认请求是不缓存的。如果要缓存获取的数据，需要手动调用设置
 *
 */
+ (void)cacheGetRequest:(BOOL)isCacheGet shoulCachePost:(BOOL)shouldCachePost;

/**
 *
 *	获取缓存总大小/bytes
 *
 *
 */
+ (unsigned long long)totalCacheSize;

/**
 *
 *	清除缓存
 */
+ (void)clearCaches;

/*!
 *
 *
 *  开启或关闭接口打印信息
 *
 *
 */
+ (void)enableInterfaceDebug:(BOOL)isDebug;

/*!
 *
 *  配置请求格式，默认为JSON。
 */
+ (void)configRequestType:(HTRequestType)requestType
             responseType:(HTResponseType)responseType
      shouldAutoEncodeUrl:(BOOL)shouldAutoEncode
  callbackOnCancelRequest:(BOOL)shouldCallbackOnCancelRequest;

/*!
 *
 *  配置公共的请求头，只调用一次即可
 *
 */
+ (void)configCommonHttpHeaders:(NSDictionary *)httpHeaders;

/**
 *
 *	取消所有请求
 */
+ (void)cancelAllRequest;
/**
 *
 *	取消某个请求
 *
 */
+ (void)cancelRequestWithURL:(NSString *)url;

/*!
 *
 *  GET请求接口，若不指定baseurl，可传完整的url
 *
 */
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;
// （有提示框）
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                          showHUD:(NSString *)statusText
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;
// 多一个params参数（无提示框）
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                           params:(NSDictionary *)params
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;
// 多一个params参数（有提示框）
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                          showHUD:(NSString *)statusText
                           params:(NSDictionary *)params
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;
// 多一个带进度回调（无提示框）
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                           params:(NSDictionary *)params
                         progress:(HTGetProgress)progress
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;
// 多一个带进度回调（有提示框）
+ (HTURLSessionTask *)getWithUrl:(NSString *)url
                     refreshCache:(BOOL)refreshCache
                          showHUD:(NSString *)statusText
                           params:(NSDictionary *)params
                         progress:(HTGetProgress)progress
                          success:(HTResponseSuccess)success
                             fail:(HTResponseFail)fail;

/*!
 *
 *  POST请求接口，若不指定baseurl，可传完整的url
 *
 */
+ (HTURLSessionTask *)postWithUrl:(NSString *)url
                      refreshCache:(BOOL)refreshCache
                            params:(NSDictionary *)params
                           success:(HTResponseSuccess)success
                              fail:(HTResponseFail)fail;

// (有提示框)
+ (HTURLSessionTask *)postWithUrl:(NSString *)url
                      refreshCache:(BOOL)refreshCache
                           showHUD:(NSString *)statusText
                            params:(NSDictionary *)params
                           success:(HTResponseSuccess)success
                              fail:(HTResponseFail)fail;
// 多一个带进度回调（无提示框）
+ (HTURLSessionTask *)postWithUrl:(NSString *)url
                      refreshCache:(BOOL)refreshCache
                            params:(NSDictionary *)params
                          progress:(HTPostProgress)progress
                           success:(HTResponseSuccess)success
                              fail:(HTResponseFail)fail;
// 多一个带进度回调（有提示框）
+ (HTURLSessionTask *)postWithUrl:(NSString *)url
                      refreshCache:(BOOL)refreshCache
                           showHUD:(NSString *)statusText
                            params:(NSDictionary *)params
                          progress:(HTPostProgress)progress
                           success:(HTResponseSuccess)success
                              fail:(HTResponseFail)fail;

/*!
 *
 *  DELETE请求接口，若不指定baseurl，可传完整的url
 */
+ (HTURLSessionTask *)deleteWithUrl:(NSString *)url
                       refreshCache:(BOOL)refreshCache
                          isShowHUD:(BOOL)showHud  success:(HTResponseSuccess)success
                               fail:(HTResponseFail)fail;
+ (HTURLSessionTask *)deleteWithUrl:(NSString *)url
                    refreshCache:(BOOL)refreshCache
                         success:(HTResponseSuccess)success
                            fail:(HTResponseFail)fail;
+ (HTURLSessionTask *)putWithUrl:(NSString *)url
                    refreshCache:(BOOL)refreshCache
                          params:(NSDictionary *)params
                         success:(HTResponseSuccess)success
                            fail:(HTResponseFail)fail;
+ (HTURLSessionTask *)putWithUrl:(NSString *)url
                    refreshCache:(BOOL)refreshCache
                       isShowHUD:(BOOL)isShowHud
                          params:(NSDictionary *)params
                         success:(HTResponseSuccess)success
                            fail:(HTResponseFail)fail;
/**
 *
 *	图片上传接口，若不指定baseurl，可传完整的url
 *
 *
 */
+ (HTURLSessionTask *)uploadWithImage:(UIImage *)image
                                   url:(NSString *)url
                              filename:(NSString *)filename
                                  name:(NSString *)name
                              mimeType:(NSString *)mimeType
                            parameters:(NSDictionary *)parameters
                              progress:(HTUploadProgress)progress
                               success:(HTResponseSuccess)success
                                  fail:(HTResponseFail)fail;



+ (HTURLSessionTask *)uploadWithImage:(UIImage *)image
                                  url:(NSString *)url
                             filename:(NSString *)filename
                            isShowHUD:(BOOL)isShowHUD
                              showHUD:(NSString *)statusText
                                 name:(NSString *)name
                             mimeType:(NSString *)mimeType
                           parameters:(NSDictionary *)parameters
                             progress:(HTUploadProgress)progress
                              success:(HTResponseSuccess)success
                                 fail:(HTResponseFail)fail;

/**
 *
 *	上传文件操作
 *
 */
+ (HTURLSessionTask *)uploadFileWithUrl:(NSString *)url
                           uploadingFile:(NSString *)uploadingFile
                                progress:(HTUploadProgress)progress
                                 success:(HTResponseSuccess)success
                                    fail:(HTResponseFail)fail;


/*!
 *
 *  下载文件
 *
 */
+ (HTURLSessionTask *)downloadWithUrl:(NSString *)url
                            saveToPath:(NSString *)saveToPath
                              progress:(HTDownloadProgress)progressBlock
                               success:(HTResponseSuccess)success
                               failure:(HTResponseFail)failure;


@end
