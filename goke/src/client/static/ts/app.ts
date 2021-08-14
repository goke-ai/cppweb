import { Dashboard } from "./views/Dashboard";
import { Posts } from "./views/Posts";
import { PostView } from "./views/PostView";
import { Settings } from "./views/Settings";

const pathToRegex = (path: string) =>
  new RegExp("^" + path.replace(/\//g, "\\/").replace(/:\w+/g, "(.+)") + "$");

const getParams = (match: {
  route: { path: string; view: any };
  result: RegExpMatchArray | null;
}): { [k: string]: string } => {
  const values = match.result?.slice(1);
  const keys = Array.from(match.route.path.matchAll(/:(\w+)/g)).map(
    (results) => results[1]
  );

  console.log(Array.from(match.route.path.matchAll(/:(\w+)/g)));

  const y = Object.fromEntries(
    keys.map((key, i) => {
      return [key, values![i]];
    })
  );

  return y;
};

const navigateTo = (url: string) => {
  history.pushState(null, "", url);
  router();
};

const router = async () => {
  const routes: { path: string; view: any }[] = [
    { path: "/", view: Dashboard },
    { path: "/posts", view: Posts },
    { path: "/posts/:id", view: PostView },
    { path: "/settings", view: Settings },
  ];

  const potentialMatches = routes.map((route) => {
    return {
      route: route,
      result: location.pathname.match(pathToRegex(route.path)),
    };
  });

  let match = potentialMatches.find((p) => p.result !== null);

  if (!match) {
    match = {
      route: routes[0],
      result: [location.pathname],
    };
  }

  const view = new match!.route.view(getParams(match));

  document.querySelector("#app")!.innerHTML = await view.getHtml();
};

window.addEventListener;

document.addEventListener("DOMContentLoaded", () => {
  document.body.addEventListener("click", (e: Event) => {
    const anchor = e.target as HTMLAnchorElement | null;
    if (anchor!.matches("[data-link]")) {
      e.preventDefault();
      navigateTo(anchor!.href);
    }
  });
  router();
});
