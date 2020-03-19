//
//  ZHNetWorking.h
//  zhonghe
//
//  Created by 韩旭 on 2017/9/7.
//  Copyright © 2017年 韩旭. All rights reserved.
//

#ifndef ZHNetWorking_h
#define ZHNetWorking_h
#import <UIKit/UIKit.h>
@interface ZHNetWorking : NSObject
@property (nonatomic) void(^popUpBack)(NSString* result);



-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading times:(NSInteger)times back:(BOOL)back success:(void(^)(id))block fail:(void(^)(id))fail_block;
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading success:(void(^)(id))block;
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading success:(void(^)(id))block fail:(void(^)(id))fail;
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading back:(BOOL)back success:(void(^)(id))block;
-(void)ZHGetHeartBeatRequest:(NSString*)interface success:(void(^)(id))block;
-(void)showAlert:(NSString*)title message:(NSString*)msg type:(BOOL)type sussess:(void(^)())block;
-(void)showAlert:(NSString*)title message:(NSString*)msg type:(BOOL)type vc:(UIViewController*)vc sussess:(void(^)())block;

@end

#endif
