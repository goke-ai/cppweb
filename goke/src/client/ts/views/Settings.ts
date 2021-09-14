import { AbstractView } from "./AbstractView";

export class Settings extends AbstractView {
  constructor(params: { [k: string]: string }) {
    super(params);
    this.setTitle("Settings");
  }

  async getHtml() {
    return `
      <h1>Settings</h1>
      <p>
          Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
          aut ducimus error, ut ipsam. Voluptatem, sed? Dolorem sit, ratione nobis 
          quos quisquam exercitationem voluptatibus, recusandae possimus expedita, 
          blanditiis rerum?
      </p>
            `;
  }
}
