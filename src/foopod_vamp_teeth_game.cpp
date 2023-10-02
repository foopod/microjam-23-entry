#include "foopod_vamp_teeth_game.h"

#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"

#include "mj/mj_game_list.h"

MJ_GAME_LIST_ADD(foopod::vamp_teeth_game)

namespace foopod
{

    vamp_teeth_game::vamp_teeth_game(int completed_games, const mj::game_data &data)
    {
        _neck.set_visible(false);
        constexpr int frames_diff = maximum_frames - minimum_frames;
        constexpr int maximum_speed_completed_games = 30;

        completed_games = bn::min(completed_games, maximum_speed_completed_games);

        int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
        _total_frames = maximum_frames - frames_reduction;
        _total_frames -= data.random.get_int(60);
        _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);
    }

    void vamp_teeth_game::fade_in([[maybe_unused]] const mj::game_data &data)
    {
    }

    mj::game_result vamp_teeth_game::play(const mj::game_data &data)
    {
        mj::game_result result;
        result.exit = data.pending_frames == 0;

        if (bn::keypad::left_held())
        {
            _velocity.set_x(_velocity.x() - _x_speed);
        }

        if (bn::keypad::right_held())
        {
            _velocity.set_x(_velocity.x() + _x_speed);
        }

        if (bn::keypad::up_held())
        {
            _velocity.set_y(_velocity.y() - _y_speed);
        }

        if (bn::keypad::down_held())
        {
            _velocity.set_y(_velocity.y() + _y_speed);
        }

        _velocity = _velocity * _friction;

        _old_state = _state;
        if (_velocity.x() > 0.2)
        {
            _state = STATE::WALK_RIGHT;
            _player.set_horizontal_flip(false);
        }
        else if (_velocity.x() < -0.2)
        {
            _state = STATE::WALK_LEFT;
            _player.set_horizontal_flip(true);
        }
        else if (_velocity.y() > 0.1)
        {
            _state = STATE::WALK_UP;
        }
        else if (_velocity.y() < -0.1)
        {
            _state = STATE::WALK_DOWN;
        }
        else
        {
            _state = STATE::IDLE;
        }

        if (_state != STATE::CAUGHT && bn::abs(_player.x() + 6 - _target.x()) < 16 && bn::abs(_player.y() + 6 - _target.y()) < 16)
        {
            _target.set_visible(false);
            _player.set_position(bn::fixed_point(_target.x() + 8, _target.y() - 8));
            _player.set_horizontal_flip(false);
            _state = STATE::CAUGHT;
            _velocity = bn::fixed_point(0, 0);
            _victory = true;
            _neck.set_visible(true);
        }

        if (_state == STATE::CAUGHT && _neck.x() > 0)
        {
            _neck.set_x(_neck.x() - 2);
        }

        if (_old_state != _state)
        {
            switch (_state)
            {
            case STATE::WALK_LEFT:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_RIGHT:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_UP:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::WALK_DOWN:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 12, bn::sprite_items::foopod_vamp.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7);
                break;
            case STATE::CAUGHT:
                _action = bn::create_sprite_animate_action_once(
                    _player, 20, bn::sprite_items::foopod_vamp.tiles_item(), 10, 10, 10, 10, 11, 11, 11, 11);
                _bite_action = bn::create_sprite_animate_action_once(
                    _bite, 40, bn::sprite_items::foopod_vamp_bite.tiles_item(), 0, 1, 2, 3);
                break;
            default:
                _action = bn::create_sprite_animate_action_forever(
                    _player, 40, bn::sprite_items::foopod_vamp.tiles_item(), 8, 8, 8, 8, 9, 9, 9, 9);
                break;
            }
        }

        _player.set_x(_player.x() + _velocity.x());
        _player.set_y(_player.y() + _velocity.y());

        if (!_bite_action.done())
        {
            _bite_action.update();
        }
        if (!_action.done())
        {
            _action.update();
        }
        else
        {
            result.exit = true;
        }

        return result;
    }

    void vamp_teeth_game::fade_out([[maybe_unused]] const mj::game_data &data)
    {
    }
}
