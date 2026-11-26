#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(ModCheckMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        static bool checked = false;
        if (checked) return true;

        auto gam = GJAccountManager::get(); 
        if (gam->m_accountID <= 0) return true;

        checked = true;

        static async::TaskHolder<web::WebResponse> s_holder;

        auto req = web::WebRequest();
        req.userAgent("");
        req.bodyString(fmt::format(
            "accountID = {}&gjp2 = {}&secret = Wmfd2893gb7",
            gam->m_accountID,
            gam->m_GJP2
        ));

        s_holder.spawn(
            req.post("https://www.boomlings.com/database/requestUserAccess.php"),
            [](web::WebResponse res) {
                auto body = res.string().unwrapOr("-1");

                if (body == "1") {
                    FLAlertLayer::create(
                        "Rating Advisor",
                        "Congrats! You have got Geometry Dash Rating Advisor!",
                        "YAY"
                    )->show();
                } else if (body == "2") {
                    FLAlertLayer::create(
                        "Moderator",
                        "Congrats! You have got Geometry Dash Moderator!",
                        "YAY"
                    )->show();
                }
            }
        );

        return true;
    }
};
