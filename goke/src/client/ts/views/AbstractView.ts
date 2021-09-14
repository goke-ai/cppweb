export abstract class AbstractView {
  params: { [k: string]: string };
  constructor(params: { [k: string]: string }) {
    this.params = params;

    console.log(params);
  }

  public setTitle(title: string) {
    document.title = title;
  }

  public async getHtml() {
    return "";
  }
}
