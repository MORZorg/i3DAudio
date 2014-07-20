//
//  OrzHrtfTildeCompatibility.m
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 20/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

#import "OrzHrtfTildeCompatibility.h"
#import "orz_hrtf~.hpp"

@implementation OrzHrtfTildeCompatibility
+(void) setup {
    orz_hrtf_tilde_setup();
}
@end
