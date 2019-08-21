//
//  HTAppDotNetAPIClient.h
//  zhonghe
//
//  Created by 韩旭 on 2017/9/7.
//  Copyright © 2017年 韩旭. All rights reserved.
//


#import <AFNetworking/AFNetworking.h>

@interface HTAppDotNetAPIClient : AFHTTPSessionManager

+ (instancetype)sharedClient;

@end
