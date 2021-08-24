import { AbstractView } from "./AbstractView";

export class Dashboard extends AbstractView {
  constructor(params: { [k: string]: string }) {
    super(params);
    this.setTitle("Dashboard");
  }

  async getHtml() {
    let resp = await fetch('dashboard.html');
    if (!resp.ok) {
      throw new Error(`HTTP error! status: ${resp.status}`);
    }
    const html = await resp.text();

    return html;
    // .then((response) => {
    //     // When the page is loaded convert it to text
    //     return response.text()
    // })
    // .then((html) => {
    //     // Initialize the DOM parser
    //     var parser = new DOMParser();

    //     // Parse the text
    //     var doc = parser.parseFromString(html, "text/html");

    //     // You can now even select part of that html as you would in the regular DOM 
    //     // Example:
    //     // var docArticle = doc.querySelector('article').innerHTML;

    //     console.log(doc);
    // })
    // .catch(function(err) {  
    //     console.log('Failed to fetch page: ', err);  
    // });
    // return `
    //   <h1>Welcome back, Goke</h1>
    //   <p>
    //       Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
    //       aut ducimus error, ut ipsam. Voluptatem, sed? Dolorem sit, ratione nobis 
    //       quos quisquam exercitationem voluptatibus, recusandae possimus expedita, 
    //       blanditiis rerum?
    //   <p>
    //   <a href="/posts" data-link="">Posts</a></p>
    //   </p>
    //   <p>
    //     Lorem ipsum dolor sit amet consectetur adipisicing elit. Ratione fugit
    //     dignissimos voluptates cum, tempora esse minima quo natus, nisi dicta eaque
    //     quisquam laudantium, unde ducimus sequi minus! Voluptas, pariatur rerum?
    //   </p>
    //         `;
  }
}
