import { AbstractView } from "./AbstractView";

export class Posts extends AbstractView {
  constructor(params: { [k: string]: string }) {
    super(params);
    this.setTitle("Posts");
  }

  async getHtml() {
    return /*html*/`
      <h1>Posts</h1>
      <p>
        Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
        aut ducimus error, ut ipsam. Voluptatem, sed? Dolorem sit, ratione nobis 
        quos quisquam exercitationem voluptatibus, recusandae possimus expedita, 
        blanditiis rerum?
      </p>        
      <a href="/posts/1" data-link="">View Post 1</a></p>

            `;
  }
}
