#include <game.h>

#include <stdlib.h>

int main(void)
{
	Game* game = calloc(1, sizeof *game);

	if (!game) {
		panic("initial malloc failed");
	}

	init_game(game, 1200, 900);

	while (likely(game->is_running)) {
		handle_game_events(game);
		update_game(game);
		render_game(game);
	}

	destroy_game(game);

	return 0;
}
