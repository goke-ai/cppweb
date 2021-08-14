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
export class Settings extends AbstractView {
    constructor() {
        super();
        this.setTitle("Settings");
    }
    getHtml() {
        return __awaiter(this, void 0, void 0, function* () {
            return `
      <h1>Settings</h1>
      <p>
          Lorem ipsum, dolor sit amet consectetur adipisicing elit. Nulla neque 
          aut ducimus error, ut ipsam. Voluptatem, sed? Dolorem sit, ratione nobis 
          quos quisquam exercitationem voluptatibus, recusandae possimus expedita, 
          blanditiis rerum?
      <p>
        <a href="/settings" data-link=""></a></p>
      </p>
            `;
        });
    }
}
//# sourceMappingURL=Settings.js.map