export default function App(){
  const root = document.createElement('div')
  root.className = 'app'
  root.innerHTML = `
    <header><h1>Noteapp (Vite)</h1></header>
    <main>
      <section class="left">
        <input id="title" placeholder="Title">
        <textarea id="body" placeholder="Write your note..."></textarea>
        <div class="actions"><button id="add">Add</button></div>
      </section>
      <section class="right">
        <ul id="list"></ul>
      </section>
    </main>
  `

  // simple client-side storage
  const STORAGE = 'noteapp.vite'
  function load(){ try{ return JSON.parse(localStorage.getItem(STORAGE)||'[]') }catch(e){return[]} }
  function save(v){ localStorage.setItem(STORAGE, JSON.stringify(v)) }

  function render(){
    const list = root.querySelector('#list'); list.innerHTML = ''
    load().slice().reverse().forEach(n=>{
      const li = document.createElement('li')
      li.innerHTML = `<strong>${n.title}</strong><div class="time">${new Date(n.time).toLocaleString()}</div><pre>${n.body}</pre>`
      const del = document.createElement('button'); del.textContent='Delete'
      del.addEventListener('click', ()=>{ save(load().filter(x=>x.id!==n.id)); render() })
      li.appendChild(del)
      list.appendChild(li)
    })
  }

  root.querySelector('#add').addEventListener('click', ()=>{
    const title = root.querySelector('#title').value || 'Untitled'
    const body = root.querySelector('#body').value || ''
    const notes = load(); notes.push({ id: Date.now(), title, body, time: Date.now() }); save(notes); render();
    root.querySelector('#title').value=''; root.querySelector('#body').value=''
  })

  render()
  return root
}
