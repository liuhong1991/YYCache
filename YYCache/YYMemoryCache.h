//
//  YYMemoryCache.h
//  YYCache <https://github.com/ibireme/YYCache>
//
//  Created by ibireme on 15/2/7.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//  内存缓存类

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 YYMemoryCache是存储键值对的快速内存缓存
 YYMemoryCache is a fast in-memory cache that stores key-value pairs.
 与NSDictionary相比,键被保留而不被复制
 In contrast to NSDictionary, keys are retained and not copied.
 与NSCache的api相似,所有的方法都是线程安全的
 The API and performance is similar to `NSCache`, all methods are thread-safe.
 
 YYMemoryCache objects differ from NSCache in a few ways:
 它通过最近最少使用的原则移除对象,NSCache的清除方法是不确定的
 * It uses LRU (least-recently-used) to remove objects; NSCache's eviction method
   is non-deterministic.
 它可以通过成本,数量和年龄来控制,NSCache的限制是不精确的.
 * It can be controlled by cost, count and age; NSCache's limits are imprecise.
 他可以被配置自动清除对象当收到内存警告或进入后台的时候
 * It can be configured to automatically evict objects when receive memory 
   warning or app enter background.
 YYMemoryCache的访问时间通常是固定的.
 The time of `Access Methods` in YYMemoryCache is typically in constant time (O(1)).
 */
@interface YYMemoryCache : NSObject

#pragma mark - Attribute
///=============================================================================
/// @name Attribute
///=============================================================================

/** The name of the cache. Default is nil. */
@property (nullable, copy) NSString *name;

/** The number of objects in the cache (read-only) */
@property (readonly) NSUInteger totalCount;

/** The total cost of objects in the cache (read-only). */
@property (readonly) NSUInteger totalCost;


#pragma mark - Limit
///=============================================================================
/// @name Limit
///=============================================================================

/**
 缓存应该保存的最大对象数
 The maximum number of objects the cache should hold.
 默认是最大值,即没有限制.
 @discussion The default value is NSUIntegerMax, which means no limit.
 这里并没有严格的限制,当缓存超过限制,一些缓存中的对象将会被在后台线程清除.
 This is not a strict limit—if the cache goes over the limit, some objects in the
 cache could be evicted later in backgound thread.
 */
@property NSUInteger countLimit;

/**
 缓存在清除对象之前所能保存的最大消耗
 The maximum total cost that the cache can hold before it starts evicting objects.
 默认没有限制,当超过限制,一些对象将会在最近的后台线程中被清除.
 @discussion The default value is NSUIntegerMax, which means no limit.
 This is not a strict limit—if the cache goes over the limit, some objects in the
 cache could be evicted later in backgound thread.
 */
@property NSUInteger costLimit;

/**
 对象在缓存中的最大到期时间
 The maximum expiry time of objects in cache.
 默认是没有限制的
 @discussion The default value is DBL_MAX, which means no limit.
 This is not a strict limit—if an object goes over the limit, the object could 
 be evicted later in backgound thread.
 */
@property NSTimeInterval ageLimit;

/**
 自动修剪检查时间为秒,默认是5.0
 The auto trim check time interval in seconds. Default is 5.0.
 缓存保存一个内部定时器以检查缓存是否达到其限制,若达到限制,则开始驱逐对象
 @discussion The cache holds an internal timer to check whether the cache reaches 
 its limits, and if the limit is reached, it begins to evict objects.
 */
@property NSTimeInterval autoTrimInterval;

/**
 默认YES,当APP接收到内存警告的时候,缓存将会移除所有的对象.
 If `YES`, the cache will remove all objects when the app receives a memory warning.
 默认为YES
 The default value is `YES`.
 */
@property BOOL shouldRemoveAllObjectsOnMemoryWarning;

/**
 默认为YES,缓存将会移除所有对象,当app进入到后台.
 If `YES`, The cache will remove all objects when the app enter background.
 The default value is `YES`.
 */
@property BOOL shouldRemoveAllObjectsWhenEnteringBackground;

/**
 默认为nil,当app收到内存警告的时候,该block将会执行
 A block to be executed when the app receives a memory warning.
 The default value is nil.
 */
@property (nullable, copy) void(^didReceiveMemoryWarningBlock)(YYMemoryCache *cache);

/**
 默认为nil,当app进入到后台,该block将会被执行.
 A block to be executed when the app enter background.
 The default value is nil.
 */
@property (nullable, copy) void(^didEnterBackgroundBlock)(YYMemoryCache *cache);

/**
 若是YES,键值对将会在主线程中被释放,否则在后台线程进行释放,默认是NO
 If `YES`, the key-value pair will be released on main thread, otherwise on
 background thread. Default is NO.
 当键值对对象包含应该在主线程释放的对象,应该设置该值为YES.
 @discussion You may set this value to `YES` if the key-value object contains
 the instance which should be released in main thread (such as UIView/CALayer).
 */
@property BOOL releaseOnMainThread;

/**
 若是YES,键值对将被异步释放以避免阻塞.否则将在访问方法中释放.默认是YES
 If `YES`, the key-value pair will be released asynchronously to avoid blocking 
 the access methods, otherwise it will be released in the access method  
 (such as removeObjectForKey:). Default is YES.
 */
@property BOOL releaseAsynchronously;


#pragma mark - Access Methods
///=============================================================================
/// @name Access Methods
///=============================================================================

/**
 返回一个布尔值只是给定的key是否存在于缓存中.
 Returns a Boolean value that indicates whether a given key is in cache.
 
 @param key An object identifying the value. If nil, just return `NO`.
 @return Whether the key is in cache.
 */
- (BOOL)containsObjectForKey:(id)key;

/**
 返回与key关联的value
 Returns the value associated with a given key.
 
 @param key An object identifying the value. If nil, just return nil.
 @return The value associated with key, or nil if no value is associated with key.
 */
- (nullable id)objectForKey:(id)key;

/**
 缓存中设置key对应的值
 Sets the value of the specified key in the cache (0 cost).
 
 @param object The object to be stored in the cache. If nil, it calls `removeObjectForKey:`.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key 
 objects that are put into it.
 */
- (void)setObject:(nullable id)object forKey:(id)key;

/**
 Sets the value of the specified key in the cache, and associates the key-value 
 pair with the specified cost.
 
 @param object The object to store in the cache. If nil, it calls `removeObjectForKey`.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 @param cost   The cost with which to associate the key-value pair.
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key
 objects that are put into it.
 */
- (void)setObject:(nullable id)object forKey:(id)key withCost:(NSUInteger)cost;

/**
 Removes the value of the specified key in the cache.
 
 @param key The key identifying the value to be removed. If nil, this method has no effect.
 */
- (void)removeObjectForKey:(id)key;

/**
 Empties the cache immediately.
 */
- (void)removeAllObjects;


#pragma mark - Trim
///=============================================================================
/// @name Trim
///=============================================================================

/**
 使用"最近最少使用"原则,从缓存中移除对象,直到"总数"低于或等于指定值
 Removes objects from the cache with LRU, until the `totalCount` is below or equal to
 the specified value.
                在缓存已修剪后允许保留总数.
 @param count  The total count allowed to remain after the cache has been trimmed.
 */
- (void)trimToCount:(NSUInteger)count;

/**
 使用"最近最少使用"原则从缓存中移除对象,直到总消耗与指定值相等
 Removes objects from the cache with LRU, until the `totalCost` is or equal to
 the specified value.
             在缓存被修剪后允许保留的总成本
 @param cost The total cost allowed to remain after the cache has been trimmed.
 */
- (void)trimToCost:(NSUInteger)cost;

/**
 使用"最近最少使用"原则从缓存中移除对象,直到所有到期的对象被指定值移除
 Removes objects from the cache with LRU, until all expiry objects removed by the
 specified value.
             对象有效期的最大期限(秒)
 @param age  The maximum age (in seconds) of objects.
 */
- (void)trimToAge:(NSTimeInterval)age;

@end

NS_ASSUME_NONNULL_END
