self.importScripts("./sw_assets");

console.log("self.assetsManifest");
console.log(self.assetsManifest);

const cacheName = `goke-v${self.assetsManifest.version}`;

const offlineAssetsInclude = [
  // /\.dll$/,
  // /\.pdb$/,
  // /\.wasm/,
  /\.html/,
  /\.js$/,
  /\.json$/,
  /\.css$/,
  // /\.woff$/,
  /\.png$/,
  /\.jpe?g$/,
  /\.gif$/,
  /\.ico$/,
  // /\.blat$/,
  // /\.dat$/,
  /\/\w+/,
];
const offlineAssetsExclude = [/^sw\.js$/, /\w+\/$/];

self.addEventListener("install", (e) => {
  console.log("[Service Worker] Install!");

  const assetsRequests = self.assetsManifest.assets
    .filter((asset) =>
      offlineAssetsInclude.some((pattern) => pattern.test(asset.url))
    )
    .filter(
      (asset) =>
        !offlineAssetsExclude.some((pattern) => pattern.test(asset.url))
    )
    .map((asset) => new Request(asset.url, { integrity: asset.hash }));

  console.log("assetsRequests");
  console.log(assetsRequests);

  e.waitUntil(
    (async () => {
      const cache = await caches.open(cacheName);
      console.log("[Service Worker] Caching all: app shell and content");
      await cache.addAll(assetsRequests);
    })()
  );
});

self.addEventListener("fetch", (e) => {
  e.respondWith(
    (async () => {
      const r = await caches.match(e.request);
      console.log(`[Service Worker] Fetching resource: ${e.request.url}`);
      if (r) {
        return r;
      }

      const response = await fetch(e.request);
      const cache = await caches.open(cacheName);
      console.log(`[Service Worker] Caching new resource: ${e.request.url}`);

      cache.put(e.request, response.clone());
      return response;
    })()
  );
});

self.addEventListener("activate", (e) => {
  e.waitUntil(
    caches.keys().then((keyList) => {
      Promise.all(
        keyList.map((key) => {
          if (key === cacheName) {
            return;
          }
          caches.delete(key);
        })
      );
    })()
  );
});

self.addEventListener("push", function (event) {
  const payload = event.data ? event.data.text() : "no payload";
  event.waitUntil(
    self.registration.showNotification("ServiceWorker Goke", {
      body: payload,
    })
  );
});
