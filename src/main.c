#include <game.h>

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	Game* game = calloc(1, sizeof *game);

	if (!game) {
		printf("%s\n", "initial malloc failed");
		return 1;
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
