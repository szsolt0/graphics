#include "game.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
	Game game = create_game(800, 600);

	while (likely(game.is_running)) {
		handle_game_events(&game);
		update_game(&game);
		render_game(&game);
	}

	destroy_game(&game);

	return 0;
}
