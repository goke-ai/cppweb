"use strict";
function getOrigin() {
    const pathArray = window.location.href.split("/");
    const protocol = pathArray[0];
    const host = pathArray[2];
    return protocol + "//" + host;
}
const originUrl = getOrigin();
console.log(originUrl);
const uri = `${originUrl}/api/contacts`;
let data;
// let contact;
let ubj = new Contact(5, "YYY", "+7016");
function displayGrid() {
    const disp = document.getElementById("grid_contacts");
    disp.innerHTML = "<ul>";
    data.forEach((c) => {
        disp.innerHTML += `<li> ${c.id}: ${c.name} -- (${c.phone}) 
              <button onclick='deleteContact(${c.id})'>DELETE</button>
              <button onclick='updateContact(${c.id}, ${JSON.stringify(ubj)})'>UPDATE</button>
            </li>`;
    });
    disp.innerHTML += "</li>";
}
function addContact(contact) {
    if (contact === undefined) {
        contact = ubj;
    }
    fetch(uri, {
        method: "POST",
        headers: {
            Accept: "application/json",
            "Content-Type": "application/json",
        },
        body: JSON.stringify(contact),
    })
        .then((response) => response.json())
        .then((data) => {
        console.log(data);
        getContacts();
    })
        .catch((error) => console.error("Unable to add contact.", error));
}
function updateContact(id, contact) {
    console.log(`update: ${contact}`);
    fetch(`${uri}/${id}`, {
        method: "PUT",
        headers: {
            Accept: "application/json",
            "Content-Type": "application/json",
        },
        body: JSON.stringify(contact),
    })
        .then((response) => response.json())
        .then((data) => {
        console.log(data);
        getContacts();
    })
        .catch((error) => console.error("Unable to add contact.", error));
}
function deleteContact(id) {
    fetch(`${uri}/${id}`, {
        method: "DELETE",
    })
        .then(() => getContacts())
        .catch((error) => console.error("Unable to delete contact.", error));
}
function getContacts() {
    fetch(uri)
        .then((response) => response.json())
        .then((resp) => {
        console.log(resp);
        data = resp.contacts;
        console.log(data);
        displayGrid();
    })
        .catch((error) => console.error("Unable to get contacts.", error));
}
//# sourceMappingURL=index.js.map