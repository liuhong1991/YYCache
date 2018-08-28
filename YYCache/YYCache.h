//
//  YYCache.h
//  YYCache <https://github.com/ibireme/YYCache>
//
//  Created by ibireme on 15/2/13.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <Foundation/Foundation.h>

#if __has_include(<YYCache/YYCache.h>)
FOUNDATION_EXPORT double YYCacheVersionNumber;
FOUNDATION_EXPORT const unsigned char YYCacheVersionString[];
#import <YYCache/YYMemoryCache.h>
#import <YYCache/YYDiskCache.h>
#import <YYCache/YYKVStorage.h>
#elif __has_include(<YYWebImage/YYCache.h>)
#import <YYWebImage/YYMemoryCache.h>
#import <YYWebImage/YYDiskCache.h>
#import <YYWebImage/YYKVStorage.h>
#else
#import "YYMemoryCache.h"
#import "YYDiskCache.h"
#import "YYKVStorage.h"
#endif

NS_ASSUME_NONNULL_BEGIN


/**
 `YYCache` is a thread safe key-value cache.
 
 It use `YYMemoryCache` to store objects in a small and fast memory cache,
 and use `YYDiskCache` to persisting objects to a large and slow disk cache.
 See `YYMemoryCache` and `YYDiskCache` for more information.
 */
@interface YYCache : NSObject

/** The name of the cache, readonly. */
@property (copy, readonly) NSString *name;

/** The underlying memory cache. see `YYMemoryCache` for more information.*/
@property (strong, readonly) YYMemoryCache *memoryCache;

/** The underlying disk cache. see `YYDiskCache` for more information.*/
@property (strong, readonly) YYDiskCache *diskCache;

/**
 Create a new instance with the specified name.
 Multiple instances with the same name will make the cache unstable.
 通过一个特殊的名字创建一个新的实例对象,不同的实例对象有相同的名字将会导致不稳定的缓存
 @param name  The name of the cache. It will create a dictionary with the name in
     the app's caches dictionary for disk cache. Once initialized you should not 
     read and write to this directory.
 @result A new cache object, or nil if an error occurs.
 */
- (nullable instancetype)initWithName:(NSString *)name;

/**
 创建一个新的实例对象通过一个不同的路径
 Create a new instance with the specified path.
 Multiple instances with the same name will make the cache unstable.
 缓存的文件全路径将会写数据,一旦初始化你不能读写这个文件夹
 @param path  Full path of a directory in which the cache will write data.
     Once initialized you should not read and write to this directory.
    返回一个新的缓存对象
 @result A new cache object, or nil if an error occurs.
 */
- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;

/**
 Convenience Initializers
 Create a new instance with the specified name.
 Multiple instances with the same name will make the cache unstable.
 
 @param name  The name of the cache. It will create a dictionary with the name in
     the app's caches dictionary for disk cache. Once initialized you should not 
     read and write to this directory.
 @result A new cache object, or nil if an error occurs.
 */
+ (nullable instancetype)cacheWithName:(NSString *)name;

/**
 Convenience Initializers
 Create a new instance with the specified path.
 Multiple instances with the same name will make the cache unstable.
 
 @param path  Full path of a directory in which the cache will write data.
     Once initialized you should not read and write to this directory.
 @result A new cache object, or nil if an error occurs.
 */
+ (nullable instancetype)cacheWithPath:(NSString *)path;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

#pragma mark - Access Methods   接口方法
///=============================================================================
/// @name Access Methods
///=============================================================================

/**
 返回一个BOOL类型的值用来指示这个key是否在内存中.
 Returns a boolean value that indicates whether a given key is in cache.
 This method may blocks the calling thread until file read finished.
 
 @param key A string identifying the value. If nil, just return NO.
 @return Whether the key is in cache.
 */
- (BOOL)containsObjectForKey:(NSString *)key;

/**
 Returns a boolean value with the block that indicates whether a given key is in cache.
 该方法立即返回,当操作结束时后台调用过去的block
 This method returns immediately and invoke the passed block in background queue
 when the operation finished.
 
 @param key   A string identifying the value. If nil, just return NO.
 @param block A block which will be invoked in background queue when finished.
 */
- (void)containsObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, BOOL contains))block;

/**
 返回和给出的key关联的value
 Returns the value associated with a given key.
 该方法可以阻止调用线程直到文件读取结束
 This method may blocks the calling thread until file read finished.
 
 @param key A string identifying the value. If nil, just return nil.
 @return The value associated with key, or nil if no value is associated with key.
 */
- (nullable id<NSCoding>)objectForKey:(NSString *)key;

/**
 返回给给出的key相关联的value
 Returns the value associated with a given key.
当操作结束时 此方法立即返回并调用后台队列中传递的块
 This method returns immediately and invoke the passed block in background queue
 when the operation finished.
 
 @param key A string identifying the value. If nil, just return nil.
 @param block A block which will be invoked in background queue when finished.
 */
- (void)objectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, id<NSCoding> object))block;

/**
 在缓存中存值
 Sets the value of the specified key in the cache.
 此方法可以组织调用线程直到文件写完成
 This method may blocks the calling thread until file write finished.
 
 @param object The object to be stored in the cache. If nil, it calls `removeObjectForKey:`.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 */
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key;

/**
 Sets the value of the specified key in the cache.
 This method returns immediately and invoke the passed block in background queue
 when the operation finished.
 
 @param object The object to be stored in the cache. If nil, it calls `removeObjectForKey:`.
 @param block  A block which will be invoked in background queue when finished.
 */
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString *)key withBlock:(nullable void(^)(void))block;

/**
 移除key对应的value
 Removes the value of the specified key in the cache.
 该方法可以组织调用线程直到文件删除结束
 This method may blocks the calling thread until file delete finished.
 
 @param key The key identifying the value to be removed. If nil, this method has no effect.
 */
- (void)removeObjectForKey:(NSString *)key;

/**
 Removes the value of the specified key in the cache.
 This method returns immediately and invoke the passed block in background queue
 when the operation finished.
 
 @param key The key identifying the value to be removed. If nil, this method has no effect.
 @param block  A block which will be invoked in background queue when finished.
 */
- (void)removeObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key))block;

/**
 Empties the cache.
 This method may blocks the calling thread until file delete finished.
 */
- (void)removeAllObjects;

/**
 Empties the cache.
 This method returns immediately and invoke the passed block in background queue
 when the operation finished.
 
 @param block  A block which will be invoked in background queue when finished.
 */
- (void)removeAllObjectsWithBlock:(void(^)(void))block;

/**
 Empties the cache with block.
 This method returns immediately and executes the clear operation with block in background.
 
 @warning You should not send message to this instance in these blocks.
 @param progress This block will be invoked during removing, pass nil to ignore.
 @param end      This block will be invoked at the end, pass nil to ignore.
 */
- (void)removeAllObjectsWithProgressBlock:(nullable void(^)(int removedCount, int totalCount))progress
                                 endBlock:(nullable void(^)(BOOL error))end;

@end

NS_ASSUME_NONNULL_END
