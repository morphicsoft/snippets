#include "coord.h"

#include <cassert>
#include <iostream>

int main()
{
    using namespace std;

    coord_system_2d world( "world" );
    world.set_origin( -10, -10 );
    world.set_extent( 20, 20 );

    coord_system_2d room( "room", &world );
    room.set_origin( 2, 1 );
    room.set_extent( 4, 4 );

    point_2d p1( 2, 2 );
    point_2d p1_in_world = transform_point( p1, world, room );

    cout << p1_in_world << "\n";

    /*
    // horizontally flipped:
    coord_system_2d flipped( "flipped", &world );
    flipped.set_origin( 90, 10 );
    flipped.set_extent( -50, 50 );

    point_2d p1( 0, 0 );

    point_2d p1_room_to_world = transform_point( p1, world, room );
    assert( p1_room_to_world.x == 10 );
    assert( p1_room_to_world.y == 10 );

    point_2d p1_flipped_to_world = transform_point( p1, world, flipped );
    assert( p1_flipped_to_world.x == 90 );
    assert( p1_flipped_to_world.y == 10 );

    point_2d p2( 10, 10 );
    point_2d p2_room_to_world = transform_point( p2, world, room );
    assert( p2_room_to_world.x == 30 );
    assert( p2_room_to_world.y == 30 );

    point_2d p2_flipped_to_world = transform_point( p2, world, flipped );
    assert( p2_flipped_to_world.x == 70 );
    assert( p2_flipped_to_world.y == 30 );

    point_2d p2_to_parent = to_parent_cs( p2, flipped );
    assert( p2_to_parent.x == 70 );
    assert( p2_to_parent.y == 30 );

    coord_system_2d centred( "centred", &world );
    centred.set_origin( -50, -50 );
    centred.set_extent( 100, 100 );

    point_2d p1_centred_to_world = to_parent_cs( p1, centred );
    assert( p1_centred_to_world.x == -50 );
    assert( p1_centred_to_world.y == -50 );

    point_2d p2_centred_to_world = to_parent_cs( p2, centred );
    assert( p2_centred_to_world.x == -40 );
    assert( p2_centred_to_world.y == -40 );

    coord_system_2d rect( "rectangular", &world );
    rect.set_origin( 0, 0 );
    rect.set_extent( 40, 30 );

    point_2d p2_rect_to_world = to_parent_cs( p2, rect );
    assert( p2_rect_to_world.x == 25 );
    assert( int(p2_rect_to_world.y) == 33 );
    */
}
