## WIP
* Fix: npc wrong behavior when ball goes bottom corner.
* Deleted: player keyboard control.
* Added: player mouse control.
* Added: particles effect.
* Updated to raylib 4.2.0
* Added: new npc ai.

## v1.0.0-alpha
* Added: fx sounds.
* Added: npc reaction time.
* Change: npc behavior based on ball position prediction.
* Change: npc movement based on easing function.
* Change: some temporal adjustments in gameplay params.
* Fix: new fps value in emscripten_set_main_loop, now the game works smooth in web browser.

## v0.1.0-alpha
* Improved: collision between ball and paddle produces a bounce angle proportional to impact distance to the paddle center.
* Added: npc paddle movement.
* Added: score system.
* Added: transition effect between scenes.
* Added: difficulty levels.
* Fixed: unload resources on window close.

## V0.0.0-alpha
* Added: basic scenes structure.
* Added: title scene.
  - Added: background.
  - Added: text message.
  - Added: Space key event.
* Added: gameplay scene.
  - Added: Paddles and ball.
  - Added: Keys events for one paddle movement.
  - Added: basic collisions.