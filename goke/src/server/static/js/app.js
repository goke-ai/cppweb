var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { Dashboard } from "./views/Dashboard";
import { Posts } from "./views/Posts";
import { Settings } from "./views/Settings";
const navigateTo = (url) => {
    history.pushState(null, "", url);
    router();
};
const router = () => __awaiter(void 0, void 0, void 0, function* () {
    const routes = [
        { path: "/", view: Dashboard },
        { path: "/posts", view: Posts },
        { path: "/settings", view: Settings },
    ];
    const potentialMatches = routes.map((route) => {
        return {
            route: route,
            isMatch: location.pathname === route.path,
        };
    });
    let match = potentialMatches.find((p) => p.isMatch);
    if (!match) {
        match = {
            route: routes[0],
            isMatch: true,
        };
    }
    const view = new match.route.view();
    document.querySelector("#app").innerHTML = yield view.getHtml();
});
window.addEventListener;
document.addEventListener("DOMContentLoaded", () => {
    document.body.addEventListener("click", (e) => {
        const anchor = e.target;
        if (anchor.matches("[data-link]")) {
            e.preventDefault();
            navigateTo(anchor.href);
        }
    });
    router();
});
//# sourceMappingURL=app.js.map