#include "zone_path.hpp"


namespace assembly::zone::path
{
    path_data_entry::path_data_entry(const path_type& type) : type (type)
    {
        switch (type)
        {
            case path_type::movement:        new(&d_movement)        path_data_movement();        break;
            case path_type::moving_platform: new(&d_moving_platform) path_data_moving_platform(); break;
            case path_type::property:        new(&d_property)        path_data_property();        break;
            case path_type::camera:          new(&d_camera)          path_data_camera();          break;
            case path_type::spawner:         new(&d_spawner)         path_data_spawner();         break;
            case path_type::showcase:        new(&d_showcase)        path_data_showcase();        break;
            case path_type::race:            new(&d_race)            path_data_race();            break;
            case path_type::rail:            new(&d_rail)            path_data_rail();            break;
        }
    }

    path_data_entry::path_data_entry(const path_data_entry& entry):
        type(entry.type), name(entry.name), value_1(entry.value_1), path_version(entry.path_version), behavior(entry.behavior)
    {
        switch (type)
        {
            case path_type::movement:        new(&d_movement)        path_data_movement();        d_movement = entry.d_movement; break;
            case path_type::moving_platform: new(&d_moving_platform) path_data_moving_platform(); d_moving_platform = entry.d_moving_platform; break;
            case path_type::property:        new(&d_property)        path_data_property();        d_property = entry.d_property; break;
            case path_type::camera:          new(&d_camera)          path_data_camera();          d_camera = entry.d_camera; break;
            case path_type::spawner:         new(&d_spawner)         path_data_spawner();         d_spawner = entry.d_spawner; break;
            case path_type::showcase:        new(&d_showcase)        path_data_showcase();        d_showcase = entry.d_showcase; break;
            case path_type::race:            new(&d_race)            path_data_race();            d_race = entry.d_race; break;
            case path_type::rail:            new(&d_rail)            path_data_rail();            d_rail = entry.d_rail; break;
        }
    }

    path_data_entry::path_data_entry(const path_data_entry&& entry):
        type(entry.type), name(entry.name), value_1(entry.value_1), path_version(entry.path_version), behavior(entry.behavior)
    {
        switch (type)
        {
            case path_type::movement:        new(&d_movement)        path_data_movement();        d_movement = entry.d_movement; break;
            case path_type::moving_platform: new(&d_moving_platform) path_data_moving_platform(); d_moving_platform = entry.d_moving_platform; break;
            case path_type::property:        new(&d_property)        path_data_property();        d_property = entry.d_property; break;
            case path_type::camera:          new(&d_camera)          path_data_camera();          d_camera = entry.d_camera; break;
            case path_type::spawner:         new(&d_spawner)         path_data_spawner();         d_spawner = entry.d_spawner; break;
            case path_type::showcase:        new(&d_showcase)        path_data_showcase();        d_showcase = entry.d_showcase; break;
            case path_type::race:            new(&d_race)            path_data_race();            d_race = entry.d_race; break;
            case path_type::rail:            new(&d_rail)            path_data_rail();            d_rail = entry.d_rail; break;
        }
    }

    path_data_entry::~path_data_entry()
    {
        switch (type)
        {
            case path_type::movement:        d_movement.~path_data_movement();               break;
            case path_type::moving_platform: d_moving_platform.~path_data_moving_platform(); break;
            case path_type::property:        d_property.~path_data_property();               break;
            case path_type::camera:          d_camera.~path_data_camera();                   break;
            case path_type::spawner:         d_spawner.~path_data_spawner();                 break;
            case path_type::showcase:        d_showcase.~path_data_showcase();               break;
            case path_type::race:            d_race.~path_data_race();                       break;
            case path_type::rail:            d_rail.~path_data_rail();                       break;
        }
    }
}


