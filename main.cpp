#include <string.h>
#include <iostream>
#include <string>

#include "core/bry_http/http_server.h"
#include "core/file_cache.h"
#include "core/http/web_application.h"

#include "app/ic_application.h"

#include "core/database/database_manager.h"

#include "database/db_init.h"

#include "core/settings.h"

#include "custom_modules/mqtt_server/mqtt_server.h"

#include <thread>

#include <SDL_ttf.h>
#include "application.h"
#include "renderer/renderer.h"
#include "app/impl_application.h"

#define MAIN_CLASS ICApplication

void create_databases() {

	//Settings *settings = Settings::get_singleton();

	//if (!settings) {
	//	printf("create_databases: Settings singleton is null!");
	//	return;
	//}

	/*
	rapidjson::Value dbs = settings->settings["databases"];

	if (!dbs.IsArray()) {
		printf("create_databases: dbs !dbs.IsArray()!");
		return;
	}
*/

	DatabaseManager *dbm = DatabaseManager::get_singleton();

	//uint32_t index = dbm->create_database("mysql");
	//Database *db = dbm->databases[0];
	//db->connect("");

	uint32_t index = dbm->create_database("sqlite");
	Database *db = dbm->databases[index];
	db->connect("database.sqlite");
}

int main(int argc, char **argv) {
	bool migrate = false;

	for (int i = 1; i < argc; ++i) {
		const char *a = argv[i];

		if (a[0] == 'm') {
			migrate = true;
		}
	}

	initialize_database_backends();

	Settings *settings = new Settings(true);

	settings->parse_file("settings.json");

	FileCache *file_cache = new FileCache(true);
	file_cache->wwwroot = "./content/www";
	file_cache->wwwroot_refresh_cache();

	DatabaseManager *dbm = new DatabaseManager();

	create_databases();

	WebApplication *app = new MAIN_CLASS();

	app->load_settings();
	app->setup_routes();
	app->setup_middleware();

	HTTPServer *server = new HTTPServer();
	server->application = app;

	server->port = 8080;
	server->initialize();

	MQTTServer *mqtt_server = new MQTTServer();
	mqtt_server->initialize();
	mqtt_server->add_local_session(
			"a/b", [](const std::string &client_id, const std::vector<uint8_t> &data, void *obj) { reinterpret_cast<ICApplication *>(obj)->mqtt_sensor_callback(client_id, data); }, app);

	TTF_Init();

	Renderer *renderer = new Renderer();
	ImplApplication *sdl_app = new ImplApplication();

	if (!migrate) {
		printf("Initialized!\n");

		//mqtt_server->run_async();
		//server->main_loop();

		while (sdl_app->running) {
			sdl_app->main_loop();
		}

	} else {
		printf("Running migrations.\n");
		app->migrate();
	}

	TTF_Quit();

	delete sdl_app;
	delete renderer;

	delete mqtt_server;
	delete server;
	delete app;
	delete dbm;
	delete file_cache;
	delete settings;

	return 0;
}