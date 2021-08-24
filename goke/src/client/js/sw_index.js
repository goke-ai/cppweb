if ("serviceWorker" in navigator) {
  navigator.serviceWorker
    .register("sw.js")
    .then((registration) => {
      return registration.pushManager
        .getSubscription()
        .then(async (subscription) => {
          if (subscription) {
            return subscription;
          }
          const response = await fetch("./vapidPublicKey");
          const vapidPublicKey = await response.text();
          const convertedVapidKey = urlBase64ToUint8Array(vapidPublicKey);
          return registration.pushManager.subscribe({
            userVisibleOnly: true,
            applicationServerKey: convertedVapidKey,
          });
        });
    })
    .then((subscription) => {
      fetch("./register", {
        method: "post",
        headers: {
          "Content-type": "application/json",
        },
        body: JSON.stringify({
          subscription: subscription,
        }),
      });

      document.getElementById("doIt").onclick = () => {
        const payload = document.getElementById("notification-payload").value;
        const delay = document.getElementById("notification-delay").value;
        const ttl = document.getElementById("notification-ttl").value;

        fetch("./sendNotification", {
          method: "post",
          headers: {
            "Content-type": "application/json",
          },
          body: JSON.stringify({
            subscription: subscription,
            payload: payload,
            delay: delay,
            ttl: ttl,
          }),
        });
      };
    });
}

const button = document.getElementById("notifications");
button.addEventListener("click", () => {
  Notification.requestPermission().then((result) => {
    if (result === "granted") {
      randomNotification();
    }
  });
});

function randomNotification() {
  const randomItem = Math.floor(Math.random() * games.length);
  const notifTitle = games[randomItem].name;
  const notifBody = `Created by ${games[randomItem].author}.`;
  const notifImg = `data/img/${games[randomItem].slug}.jpg`;
  const options = {
    body: notifBody,
    icon: notifImg,
  };
  new Notification(notifTitle, options);
  setTimeout(randomNotification, 30000);
}
