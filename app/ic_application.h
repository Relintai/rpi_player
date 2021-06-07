#ifndef IC_APPLICATION_H
#define IC_APPLICATION_H

#include <vector>

#include "core/http/web_application.h"
#include "core/object.h"

#include "modules/message_page/message_page.h"
#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

class ICApplication : public WebApplication {
public:
    static void index(Object *instance, Request *request);
    static void get_sensor_data(Object *instance, Request *request);
    
    static void app_docs_page(Object *instance, Request *request);
    static void engine_docs_page(Object *instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    virtual void migrate();

    void load_md(const std::string &file_name, std::string *str);

    ICApplication();
    ~ICApplication();

    static std::string engine_docs;
    static std::string app_docs;
};

#endif