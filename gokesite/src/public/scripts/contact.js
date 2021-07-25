const uri = 'api/contacts';
let contacts = [];

function getContacts() {
  fetch(uri)
    .then((response) => response.json())
    .then((data) => _displayContacts(data.contacts))
    .catch((error) => console.error("Unable to get contacts.", error));
}

function addContact() {
  const addIdTextbox = document.getElementById('add-id');
  const addNameTextbox = document.getElementById('add-name');
  const addPhoneTextbox = document.getElementById('add-phone');

  const contact = {
    id: parseInt(addIdTextbox.value.trim()),
    name: addNameTextbox.value.trim(),
    phone: addPhoneTextbox.value.trim()
  };

  fetch(uri, {
    method: 'POST',
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(contact)
  })
    .then(response => response.json())
    .then(() => {
      getContacts();
      addIdTextbox.value = 0;
      addNameTextbox.value = '';
      addPhoneTextbox.value = '';
    })
    .catch(error => console.error('Unable to add contact.', error));
}

function deleteContact(id) {
  fetch(`${uri}/${id}`, {
    method: 'DELETE'
  })
  .then(() => getContacts())
  .catch(error => console.error('Unable to delete contact.', error));
}

function displayEditForm(id) {
  console.log('displayEditForm: ' + id);
  console.log(contacts);

  const contact = contacts.find(contact => contact.id === id.toString());

  console.log(contact);
  
  document.getElementById('edit-id').value = contact.id;
  document.getElementById('edit-name').value = contact.name;
  document.getElementById('edit-phone').value = contact.phone;
  document.getElementById('editForm').style.display = 'block';
}

function updateContact() {
  const contactId = document.getElementById('edit-id').value;
  const contact = {
    id: parseInt(contactId, 10),
    name: document.getElementById('edit-name').value.trim(),
    phone: document.getElementById('edit-phone').value.trim()
  };

  fetch(`${uri}/${contactId}`, {
    method: 'PUT',
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(contact)
  })
  .then(() => getContacts())
  .catch(error => console.error('Unable to update contact.', error));

  closeInput();

  return false;
}

function closeInput() {
  document.getElementById('editForm').style.display = 'none';
}

function _displayCount(contactCount) {
  const name = contactCount === 1 ? "to-do" : "to-dos";

  document.getElementById("counter").innerText = `${contactCount} ${name}`;
}

function _displayContacts(data) {

  console.log(data);

  const tBody = document.getElementById("contacts");
  tBody.innerHTML = "";

  _displayCount(data.length);

  const button = document.createElement("button");

  data.forEach((contact) => {
    let isCompleteCheckbox = document.createElement("input");
    isCompleteCheckbox.type = "checkbox";
    isCompleteCheckbox.disabled = true;
    isCompleteCheckbox.checked = contact.isComplete;

    let editButton = button.cloneNode(false);
    editButton.innerText = "Edit";
    editButton.setAttribute("onclick", `displayEditForm(${contact.id})`);

    let deleteButton = button.cloneNode(false);
    deleteButton.innerText = "Delete";
    deleteButton.setAttribute("onclick", `deleteContact(${contact.id})`);

    let tr = tBody.insertRow();

    let td0 = tr.insertCell(0);
    td0.appendChild(isCompleteCheckbox);

    let td1 = tr.insertCell(1);
    let textNode1 = document.createTextNode(contact.id);
    td1.appendChild(textNode1);

    let td2 = tr.insertCell(2);
    let textNode2 = document.createTextNode(contact.name);
    td2.appendChild(textNode2);

    let td3 = tr.insertCell(3);
    let textNode3 = document.createTextNode(contact.phone);
    td3.appendChild(textNode3);

    let td4 = tr.insertCell(4);
    td4.appendChild(editButton);

    let td5 = tr.insertCell(5);
    td5.appendChild(deleteButton);
  });

  contacts = data;
}