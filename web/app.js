const STORAGE_KEY = 'noteapp.notes'

function loadNotes() {
  try { return JSON.parse(localStorage.getItem(STORAGE_KEY) || '[]') }
  catch (e) { return [] }
}

function saveNotes(notes) { localStorage.setItem(STORAGE_KEY, JSON.stringify(notes)) }

function render() {
  const list = document.getElementById('list')
  list.innerHTML = ''
  const notes = loadNotes()
  notes.slice().reverse().forEach(n => {
    const li = document.createElement('li')
    li.className = 'note'
    const h = document.createElement('h3'); h.textContent = n.title
    const p = document.createElement('pre'); p.textContent = n.body
    const meta = document.createElement('div'); meta.className = 'meta'; meta.textContent = new Date(n.time).toLocaleString()
    const del = document.createElement('button'); del.textContent = 'Delete'; del.className = 'del'
    del.addEventListener('click', () => {
      if (!confirm('Delete this note?')) return
      const remaining = loadNotes().filter(x => x.id !== n.id)
      saveNotes(remaining)
      render()
    })
    li.appendChild(h); li.appendChild(meta); li.appendChild(p); li.appendChild(del)
    list.appendChild(li)
  })
}

function addNote() {
  const title = document.getElementById('title').value.trim() || 'Untitled'
  const body = document.getElementById('body').value.trim()
  const notes = loadNotes()
  const id = Date.now() + Math.floor(Math.random()*1000)
  notes.push({ id, title, body, time: Date.now() })
  saveNotes(notes)
  document.getElementById('title').value = ''
  document.getElementById('body').value = ''
  render()
}

function exportJSON() {
  const data = localStorage.getItem(STORAGE_KEY) || '[]'
  const blob = new Blob([data], {type: 'application/json'})
  const url = URL.createObjectURL(blob)
  const a = document.createElement('a')
  a.href = url; a.download = 'notes.json'; document.body.appendChild(a); a.click(); a.remove(); URL.revokeObjectURL(url)
}

function importJSON(file) {
  const r = new FileReader();
  r.onload = () => {
    try {
      const incoming = JSON.parse(r.result)
      if (!Array.isArray(incoming)) throw new Error('invalid')
      const notes = loadNotes().concat(incoming)
      saveNotes(notes)
      render()
    } catch (e) { alert('Invalid JSON') }
  }
  r.readAsText(file)
}

document.addEventListener('DOMContentLoaded', () => {
  document.getElementById('add').addEventListener('click', addNote)
  document.getElementById('export').addEventListener('click', exportJSON)
  document.getElementById('importBtn').addEventListener('click', () => document.getElementById('importFile').click())
  document.getElementById('importFile').addEventListener('change', (e) => { if (e.target.files[0]) importJSON(e.target.files[0]) })
  render()
})
