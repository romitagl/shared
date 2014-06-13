#import "ObjCtoCPlusPlus.h"

#import "CPlusPlus.h"


@implementation Performance_ObjCtoCPlusPlus

+ (void) sortArrayCPlusPlus: (NSInteger) array_size
{
    Performance_CPlusPlus::sortArray((unsigned int)array_size);
}
@end