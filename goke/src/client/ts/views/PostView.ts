import { AbstractView } from "./AbstractView";

export class PostView extends AbstractView {
  constructor(params: { [k: string]: string }) {
    super(params);
    this.setTitle("Viewing Post");
  }

  async getHtml() {
    console.log(this.params.id);
    return `
    <h1>Viewing Post</h1>
    <p>
      Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
      blanditiis rerum?
    </p>
    <img src="/images/logo512.png" alt="logo" style="width: 200px">
                `;
  }
}
