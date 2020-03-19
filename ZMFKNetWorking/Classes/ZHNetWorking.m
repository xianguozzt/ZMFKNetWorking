//
//  ZHNetWorking.m
//  zhonghe
//
//  Created by 韩旭 on 2017/9/7.
//  Copyright © 2017年 韩旭. All rights reserved.
//
#import "ZHNetWorking.h"
#import "HTNetWorking.h"
#import "ZHShowMessageView.h"
@interface ZHNetWorking()
@property (nonatomic) NSDictionary *headerDic;
@property (nonatomic) NSString *baseUrl;
@property (nonatomic) UIViewController *lastVC;
@property (nonatomic) UIAlertController *alertVC;
@end
@implementation ZHNetWorking
static ZHNetWorking *_instance;
static BOOL isPopup = false;
+(instancetype)allocWithZone:(struct _NSZone *)zone
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (_instance == nil) {
            _instance = [super allocWithZone:zone];
        }
    });
    return _instance;
}
#define TIMEOUT_TIMES                   2
- (BOOL)getDelegateStatus
{
    NSDictionary *proxySettings = CFBridgingRelease((__bridge CFTypeRef _Nullable)((__bridge NSDictionary *)CFNetworkCopySystemProxySettings()));
    NSArray *proxies = CFBridgingRelease((__bridge CFTypeRef _Nullable)((__bridge NSArray *)CFNetworkCopyProxiesForURL((__bridge CFURLRef)[NSURL URLWithString:@"http://www.google.com"], (__bridge CFDictionaryRef)proxySettings)));
    NSDictionary *settings = [proxies objectAtIndex:0];
    NSLog(@"host=%@", [settings objectForKey:(NSString *)kCFProxyHostNameKey]);
    NSLog(@"port=%@", [settings objectForKey:(NSString *)kCFProxyPortNumberKey]);
    NSLog(@"type=%@", [settings objectForKey:(NSString *)kCFProxyTypeKey]);
    if ([[settings objectForKey:(NSString *)kCFProxyTypeKey] isEqualToString:@"kCFProxyTypeNone"])
    {
        return NO;
        
    } else {
        [self makeToast:@"网络已断开，请重新选择wifi"];
        return YES;
    }
}
-(void)makeToast:(NSString *)str{
    if(self.popUpBack){
        self.popUpBack(str);
    }
}
-(void)showAlert:(NSString*)title message:(NSString*)msg type:(BOOL)type sussess:(void(^)())block{
    [self showAlert:title message:msg type:type vc:[self currentViewController] sussess:block];
}
-(void)showAlert:(NSString*)title message:(NSString*)msg type:(BOOL)type vc:(UIViewController*)vc sussess:(void(^)())block{
    if([self.lastVC isKindOfClass:vc]){
        return;
    }
    [self.alertVC dismissViewControllerAnimated:NO completion:nil];
    self.lastVC = vc;
    UIAlertController *alertVC = [UIAlertController alertControllerWithTitle:title message:msg preferredStyle:UIAlertControllerStyleAlert];
    self.alertVC = alertVC;
    if([[self currentViewController] isKindOfClass:NSClassFromString(@"ZHLoginSmsCodeViewController")]||[[self currentViewController] isKindOfClass:NSClassFromString(@"ZHLoginBindMobileController")]){
        alertVC.view.backgroundColor = [UIColor whiteColor];
    }else{
        id vc = [[NSClassFromString(@"ZHMainFontModel") alloc] init];
        UIColor *color = [vc valueForKey:@"_color11"];
        if (color){
            alertVC.view.backgroundColor = color;
        }
    }
    if(type == YES){
        UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
        [alertVC addAction:cancelAction];
    }
    UIAlertAction *sureAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        if(block)
            block();
        self.lastVC = nil;
    }];
    
    [alertVC addAction:sureAction];
    [vc presentViewController:alertVC animated:YES completion:nil];
}
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading success:(void(^)(id))block fail:(void(^)(id))fail{
    [self ZHGetRequest:interface method:method para:para isLoading:isLoading times:TIMEOUT_TIMES  back:NO success:block fail:fail];
}
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading success:(void(^)(id))block{
    [self ZHGetRequest:interface method:method para:para isLoading:isLoading times:TIMEOUT_TIMES  back:NO success:block fail:nil];
}

-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading back:(BOOL)back success:(void(^)(id))block{
    [self ZHGetRequest:interface method:method para:para isLoading:isLoading times:TIMEOUT_TIMES  back:back success:block fail:nil];
}

-(void)ZHGetHeartBeatRequest:(NSString*)interface success:(void(^)(id))block{
    NSDictionary *dic = @{@"enablePush":@([[UIApplication sharedApplication] currentUserNotificationSettings].types  != UIRemoteNotificationTypeNone)};
    [self ZHGetRequest:interface method:@"post" para:dic isLoading:NO times:CGFLOAT_MAX  back:YES success:block fail:nil];
}
-(void)ZHGetRequest:(NSString*)interface method:(NSString*)method para:(NSDictionary*)para isLoading:(BOOL)isLoading times:(NSInteger)times back:(BOOL)back success:(void(^)(id))block fail:(void(^)(id))fail_block{
//#ifndef DEBUG
//    if([self getDelegateStatus]){
//        return;
//    }
//#endif
    NSString *requestUrl;
    __weak typeof(self) weakSelf = self;
    if([interface hasPrefix:@"http"]){
        requestUrl = interface;
    }else{
        if(!self.baseUrl){
            return;
        }
        requestUrl =[NSString stringWithFormat:@"%@%@",self.baseUrl,interface];
    }
    
    NSDictionary *dic = self.headerDic;
    [HTNetWorking configCommonHttpHeaders:dic];
    NSInteger type = 0;
    if([method isEqualToString:@"get"]){
        type = 1;
    }
    type = (type<<1)+isLoading;
    HTResponseSuccess suc =^(id response){
        if(response == NULL){
            if(back == NO){
                if(isPopup == NO){
                    [weakSelf showAlert:@"" message:@"服务器繁忙，请稍后再试。" type:NO sussess:^(){
                        isPopup = NO;
                    }];
                    isPopup = YES;
                }
            }
        }else{
            if([self ZHJudgement:response element:@"errMsg"]&&isLoading == YES){
                //[[self currentViewController].view makeToast:response[@"errMsg"] duration:2 position:CSToastPositionCenter];
                [weakSelf makeToast:response[@"errMsg"]];
            }
            if(response[@"rtn"]){
                NSInteger rtn = [response[@"rtn"] integerValue];
                if(rtn == 0){
                    if(block){
                        block(response);
                    }
                }else{
                    switch (rtn) {
                        case 30007:
                        {
                            [HTNetWorking cancelAllRequest];
                            if(back == NO){
                                [weakSelf showAlert:@"" message:response[@"errMsg"] type:NO sussess:^(){
                                    Class vc = NSClassFromString(@"ZHLoginViewController");
                                    id viewcontroller = [[vc alloc] init];
                                    
                                    if (vc) {
                                        NSString *name = [[NSBundle mainBundle] bundleIdentifier];
                                        if([name containsString:@"zhonghe"]){
                                            UINavigationController *navi = [[UINavigationController alloc] initWithRootViewController:viewcontroller];
                                            [[weakSelf currentViewController] presentViewController:navi animated:YES completion:nil];
                                        }else{
                                            [[weakSelf currentViewController] presentViewController:viewcontroller animated:YES completion:nil];
                                        }
                                        
                                    }
                                }];
                            }
                            
                            return;
                            
                        }
                            break;
                        case 80003:
                        case 30014:
                        {
                            if(block){
                                block(response);
                            }
                            return;
                        }
                            break;
                    }
                }
            }else{
                //[[self currentViewController].view makeToast:@"服务器开小差去了，请稍后再试。" duration:2 position:CSToastPositionCenter];
                [weakSelf makeToast:@"服务器开小差去了，请稍后再试。"];
            }
            
        }
    };
    HTResponseFail fail =^(NSError *error) {
        NSLog(@"%@",error);
        NSInteger lefttimes = times;
        lefttimes --;
        if(lefttimes > 0){
            [weakSelf ZHGetRequest:interface method:method para:para isLoading:isLoading times:lefttimes back:back success:block fail:fail_block];
            
        }else{
            [ZHShowMessageView dismiss];
            if(back == YES){
                return;
            }
            if(fail_block){
                fail_block(error);
            }else{
                NSString *str = @"服务器繁忙，请稍后再试。";
                if(isPopup == NO){
                    if(back == NO){
                        
                        if(error.userInfo[@"NSLocalizedDescription"]){
                            if ([error.userInfo[@"NSLocalizedDescription"] hasPrefix:@"似乎已断开与互联网的连接"]){
                                str = @"网络连接不可用";
                            }
                            else if([error.userInfo[@"NSLocalizedDescription"] hasPrefix:@"已取消"]){
                                return;
                            }
                            
                        }
                        
                        [weakSelf showAlert:@"" message:str type:NO sussess:^(){
                            isPopup = NO;
                        }];
                        
                    }
                    isPopup = YES;
                }else{
                    //[[self currentViewController].view makeToast:str duration:2 position:CSToastPositionCenter];
                    [weakSelf makeToast:str];
                }
            }
        }
    };
    switch (type) {
        case 0:
        {
            [HTNetWorking postWithUrl:requestUrl refreshCache:YES params:para success:suc fail:fail];
            
        }
            break;
        case 1:
        {
            
            [HTNetWorking postWithUrl:requestUrl refreshCache:YES showHUD:@"努力加载中" params:para success:suc fail:fail];
        }
            break;
        case 2:
        {
            [HTNetWorking getWithUrl:requestUrl refreshCache:YES success:suc fail:fail];
        }
            break;
        case 3:
        {
            [HTNetWorking getWithUrl:requestUrl refreshCache:YES showHUD:@"努力加载中" success:suc fail:fail];
        }
            break;
        default:
            break;
    }
}
-(BOOL)ZHJudgement:(NSDictionary*)dic element:(NSString*)str{
    if (dic == nil){
        return NO;
    }
    id value = [dic objectForKey:str];
    if(!value){
        return NO;
    }
    if ([value isEqual:[NSNull null]]){
        return NO;
    }
    if([value isKindOfClass:[NSString class]]){
        if([value isEqualToString:@""]){
            return NO;
        }
    }
    if([value isKindOfClass:[NSArray class]]){
        if(((NSArray*)value).count == 0){
            return NO;
        }
    }
    
    return YES;
}
- (UIViewController *)currentViewController {
    UIViewController *resultVC;
    resultVC = [self _topViewController:[[UIApplication sharedApplication].keyWindow rootViewController]];
    while (resultVC.presentedViewController) {
        resultVC = [self _topViewController:resultVC.presentedViewController];
    }
    return resultVC;
}

- (UIViewController *)_topViewController:(UIViewController *)vc {
    if ([vc isKindOfClass:[UINavigationController class]]) {
        return [self _topViewController:[(UINavigationController *)vc topViewController]];
    } else if ([vc isKindOfClass:[UITabBarController class]]) {
        return [self _topViewController:[(UITabBarController *)vc selectedViewController]];
    } else {
        return vc;
    }
    return nil;
}
@end
