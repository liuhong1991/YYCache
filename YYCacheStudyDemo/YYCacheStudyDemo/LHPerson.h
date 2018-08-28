//
//  LHPerson.h
//  YYCacheStudyDemo
//
//  Created by 刘洪 on 2018/8/27.
//  Copyright © 2018年 刘洪. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LHPerson : NSObject
//-(instancetype)init UNAVAILABLE_ATTRIBUTE;
+(instancetype)new UNAVAILABLE_ATTRIBUTE;
-(instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;

@end
