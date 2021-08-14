var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { AbstractView } from "./AbstractView";
export class Dashboard extends AbstractView {
    constructor() {
        super();
        this.setTitle("Dashboard");
    }
    getHtml() {
        return __awaiter(this, void 0, void 0, function* () {
            return `
      <h1>Welcome back, Goke</h1>
      <p>
          Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
          aut ducimus error, ut ipsam. Voluptatem, sed? Dolorem sit, ratione nobis 
          quos quisquam exercitationem voluptatibus, recusandae possimus expedita, 
          blanditiis rerum?
      <p>
      <a href="/posts" data-link="">Posts</a></p>
      </p>
      <p>
        Lorem ipsum dolor sit amet consectetur adipisicing elit. Ratione fugit
        dignissimos voluptates cum, tempora esse minima quo natus, nisi dicta eaque
        quisquam laudantium, unde ducimus sequi minus! Voluptas, pariatur rerum?
      </p>
            `;
        });
    }
}
//# sourceMappingURL=Dashboard.js.map