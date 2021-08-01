
const uri: string = "http://localhost:18080/api/contacts";

let data: Contact[];
// let contact;
let ubj: Contact = new Contact(5, "YYY", "+7016");

function displayGrid(): void {
  const disp: HTMLElement | null = document.getElementById("grid_contacts");
  disp!.innerHTML = "<ul>";

  data.forEach((c) => {
    disp!.innerHTML += `<li> ${c.id}: ${c.name} -- (${c.phone}) 
              <button onclick='deleteContact(${c.id})'>DELETE</button>
              <button onclick='updateContact(${c.id}, ${JSON.stringify(
      ubj
    )})'>UPDATE</button>
            </li>`;
  });

  disp!.innerHTML += "</li>";
}

function addContact(contact: Contact) {
  fetch(uri, {
    method: "POST",
    headers: {
      Accept: "application/json",
      "Content-Type": "application/json",
    },
    body: JSON.stringify(contact),
  })
    .then((response) => response.json())
    .then((data: Contact[]) => {
      console.log(data);
      getContacts();
    })
    .catch((error) => console.error("Unable to add contact.", error));
}

function updateContact(id: number, contact: Contact) {

    console.log(contact);

  fetch(`${uri}/${id}`, {
    method: "PUT",
    headers: {
      Accept: "application/json",
      "Content-Type": "application/json",
    },
    body: JSON.stringify(contact),
  })
    .then((response) => response.json())
    .then((data: Contact) => {
      console.log(data);
      getContacts();
    })
    .catch((error) => console.error("Unable to add contact.", error));
}

function deleteContact(id: number) {
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
