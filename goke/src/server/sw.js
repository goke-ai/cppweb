self.importScripts("./sw_assets");

console.log("self.assetsManifest.assets");
console.log(self.assetsManifest.assets);

// const cacheNamePrefix = 'offline-cache-';
// const cacheName = `${cacheNamePrefix}${self.assetsManifest.version}`;
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
  console.log("Install!");

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
    caches.open("static").then((cache) => {
      return cache.addAll(
        assetsRequests
        //   [
        //   "./",
        //   "./css/app.css",
        //   "./images/pix.jpg",
        //   "./images/logo512.png",
        //   "./js/bundle.js",
        // ]
      );
    })
  );
});

self.addEventListener("activate", (e) => {
  console.log(`Activated for ${e}`);
  console.log("clients claim", self.clients.claim());
  console.log(self.clients.claim());

  return self.clients.claim();
});

self.addEventListener("fetch", (e) => {
  console.log(`Intercepting fetch request for ${e.request.url}`);
  e.respondWith(
    caches.match(e.request).then((response) => {

      console.log("response");
      console.log(response);
      
      return response || fetch(e.request);
    })
  );
});
